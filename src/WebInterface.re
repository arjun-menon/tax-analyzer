let rs = ReasonReact.string;

let checkYear = (year: int): bool => {
  switch (Belt.Array.getBy(TaxRates.availableYears, aYear => aYear == year)) {
  | Some(_) => true
  | None => false
  };
};

// Note 1: any comaprison against NaN yields a false-like value. Negating this false-like value with `!` does not work.
// Note 2: the seemingly unnecessary `? true : false` turns a potential false-like value into an actual ReML false bool.
let checkIncome = (income: float): bool => income >= 0.0 ? true : false;
let checkDeductions = (deductions: float): bool => deductions >= 0.0 ? true : false;

let checkExemptions = (exemptions: int): bool => exemptions >= 0;

let checkParams = (params: TaxCalc.taxParams): bool =>
  checkYear(params.year)
  && checkIncome(params.income)
  && checkDeductions(params.deductions)
  && checkExemptions(params.exemptions);

let getParameterByNameRaw: (string, string) => string = [%bs.raw
  {|
  function getParameterByName(query, name) {
    // based on: http://stackoverflow.com/a/901144/908430
    name = name.replace(/[\[]/, '\\[').replace(/[\]]/, '\\]');
    const regex = new RegExp('[\\?&]' + name + '=([^&#]*)');
    const results = regex.exec('?' + query);
    return results == null ? '' : results[1].replace(/\+/g, ' ');
  }
|}
];

let getParameterByName = (query: string, name: string) =>
  Js_global.decodeURIComponent(getParameterByNameRaw(query, name));

let removeCommas: string => string = [%bs.raw
  {|
  function removeCommas(text) {
    return text.replace(/,/g, '');
  }
|}
];

let parseFloat = (s: string): float => Js.Float.fromString(removeCommas(s));

let parseInt: string => int = [%raw {| x => {
  const v = parseInt(x, 10);
  return isNaN(v) ? 0 : v;
} |}];

let destringBool = (s: string) => s == "true"; // since bool_of_string throws an exception

let getParamsFromStrings =
    (year: string, income: string, deductions: string, exemptions: string, excludeEmp: string): TaxCalc.taxParams => {
  year: parseInt(year),
  income: parseFloat(income),
  deductions: parseFloat(deductions),
  exemptions: parseInt(exemptions),
  excludeEmp: destringBool(excludeEmp),
};

type urlParams = {
  year: string,
  income: string,
  deductions: string,
  exemptions: string,
  excludeEmp: string,
};

let toUrlParams = (params: TaxCalc.taxParams): urlParams => {
  year: string_of_int(params.year),
  income: Js.Float.toString(params.income),
  deductions: Js.Float.toString(params.deductions),
  exemptions: string_of_int(params.exemptions),
  excludeEmp: string_of_bool(params.excludeEmp),
};

let makeUrlParams = (params: TaxCalc.taxParams): string => {
  let p: urlParams = toUrlParams(params);
  "?year="
  ++ Js_global.encodeURIComponent(p.year)
  ++ "&income="
  ++ Js_global.encodeURIComponent(p.income)
  ++ "&deductions="
  ++ Js_global.encodeURIComponent(p.deductions)
  ++ "&exemptions="
  ++ Js_global.encodeURIComponent(p.exemptions)
  ++ "&excludeEmp="
  ++ Js_global.encodeURIComponent(p.excludeEmp);
};

let getUrlParams = (url: ReasonReactRouter.url): urlParams => {
  let urlParams: string = url.search;
  {
    year: getParameterByName(urlParams, "year"),
    income: getParameterByName(urlParams, "income"),
    deductions: getParameterByName(urlParams, "deductions"),
    exemptions: getParameterByName(urlParams, "exemptions"),
    excludeEmp: getParameterByName(urlParams, "excludeEmp"),
  };
};

let destringUrlParams = (urlParams: urlParams): TaxCalc.taxParams =>
  getParamsFromStrings(
    urlParams.year,
    urlParams.income,
    urlParams.deductions,
    urlParams.exemptions,
    urlParams.excludeEmp,
  );

let fixUrlIfNecessary = (defaultParams: TaxCalc.taxParams) => {
  // test with: http://localhost:8000/?income=150%2C000&deductions=7000&exemptions=3&taxYear=2019
  // test with: http://localhost:8000/?year=2019&income=NaN&deductions=7000&exemptions=3
  let url = ReasonReactRouter.dangerouslyGetInitialUrl();
  if (url.search === "") {
    ReasonReactRouter.replace(makeUrlParams(defaultParams));
  } else {
    let params = ref(destringUrlParams(getUrlParams(url)));
    if (!checkYear(params^.year)) {
      params := {...params^, year: defaultParams.year};
    };
    if (!checkIncome(params^.income)) {
      params := {...params^, income: defaultParams.income};
    };
    if (!checkDeductions(params^.deductions)) {
      params := {...params^, deductions: defaultParams.deductions};
    };
    if (!checkExemptions(params^.exemptions)) {
      params := {...params^, exemptions: defaultParams.exemptions};
    };
    ReasonReactRouter.replace(makeUrlParams(params^));
  };
};

let eventS = (setValue, event) => {
  let value: string = ReactEvent.Form.target(event)##value;
  setValue(_ => value);
};

let areParamsDifferent = (p1: TaxCalc.taxParams, p2: TaxCalc.taxParams) =>
  p1.year != p2.year
  || p1.income != p2.income
  || p1.deductions != p2.deductions
  || p1.exemptions != p2.exemptions
  || p1.excludeEmp != p2.excludeEmp;

let determineDataPoints = (income: float): array(int) => {
  let incomeInt = int_of_float(income);
  let stepMaybe: int = int_of_float(income /. 12.5);
  let uptoMaybe: int = incomeInt * 2;
  let stepMin: int = 10000;
  let uptoMin: int = 250000;
  let step: int = Pervasives.max(stepMin, stepMaybe);
  let upto: int = Pervasives.max(uptoMin, uptoMaybe);

  let lowerHalf: array(int) = Belt.Array.rangeBy(0, incomeInt - 1, ~step);
  let lastElem: option(int) = Belt.Array.get(lowerHalf, Belt.Array.length(lowerHalf) - 1);
  let nextAmt: int =
    (
      switch (lastElem) {
      | Some(x) => x
      | None => 0
      }
    )
    + step;
  let middle: array(int) = incomeInt == nextAmt ? [||] : [|incomeInt|];
  let upperHalf: array(int) = Belt.Array.rangeBy(nextAmt, upto, ~step);

  Belt.Array.concatMany([|lowerHalf, middle, upperHalf|]);
};

[@bs.deriving abstract]
type dataPoint = {
  effective: float,
  marginal: float,
  income: string,
};

let toDataPoint = (params: TaxCalc.taxParams): dataPoint => {
  let taxes = TaxCalc.calcTaxes(params);
  let marginalTaxRate = TaxReport.calcMarginalTaxRate(params, taxes);
  let round = (n: float): float => parseFloat(TaxReport.twoPointFloatRepr(n));
  dataPoint(
    ~effective=round(taxes.effectiveTaxRate),
    ~marginal=round(marginalTaxRate),
    ~income=TaxReport.nsiof(taxes.income),
  );
};

module TaxRateChart = {
  [@react.component]
  let make = (~params: TaxCalc.taxParams) => {
    let incomeIntS = TaxReport.nsiof(params.income);
    let points: array(int) = determineDataPoints(params.income);
    let toStepParams = (income: float): TaxCalc.taxParams => {...params, income};
    let data: array(dataPoint) =
      points->Belt.Array.map(float_of_int)->Belt.Array.map(toStepParams)->Belt.Array.map(toDataPoint);

    let marginalRateColor = "rgb(256,0,0)";
    let effectiveRateColor = "rgb(0,0,256)";
    let referenceLineColor = "rgb(0,256,0)";

    <>
      <h1> {rs("Tax Rate Visualizer")} </h1>
      <center>
        <BsRecharts.AreaChart width=600 height=400 data>
          <BsRecharts.CartesianGrid strokeDasharray="3 3" />
          <BsRecharts.XAxis dataKey="income" />
          <BsRecharts.YAxis />
          <BsRecharts.Tooltip />
          <BsRecharts.Legend verticalAlign=`top height=36 />
          <BsRecharts.Area
            _type=`monotone
            dataKey="marginal"
            unit="%"
            name="Marginal Tax Rate"
            stackId="1"
            legendType=`circle
            stroke=marginalRateColor
            fill=marginalRateColor
          />
          <BsRecharts.Area
            _type=`monotone
            dataKey="effective"
            unit="%"
            name="Effective Tax Rate"
            stackId="2"
            legendType=`circle
            stroke=effectiveRateColor
            fill=effectiveRateColor
          />
          <BsRecharts.ReferenceLine x=incomeIntS stroke=referenceLineColor />
        </BsRecharts.AreaChart>
      </center>
    </>;
  };
};

module TaxAnalyzer = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    let urlParams: urlParams = getUrlParams(url);
    let params: TaxCalc.taxParams = destringUrlParams(urlParams);

    let (yearS, setYearS) = React.useState(() => urlParams.year);
    let (incomeS, setIncomeS) = React.useState(() => urlParams.income);
    let (deductionsS, setDeductionsS) = React.useState(() => urlParams.deductions);
    let (exemptionsS, setExemptionsS) = React.useState(() => urlParams.exemptions);
    let (excludeEmpB, setExcludeEmpB) = React.useState(() => destringBool(urlParams.excludeEmp));

    let formParams: TaxCalc.taxParams =
      getParamsFromStrings(yearS, incomeS, deductionsS, exemptionsS, string_of_bool(excludeEmpB));

    React.useEffect0(() => {
      let watcherId =
        ReasonReactRouter.watchUrl(url => {
          let p = getUrlParams(url);
          setYearS(_ => p.year);
          setIncomeS(_ => p.income);
          setDeductionsS(_ => p.deductions);
          setExemptionsS(_ => p.exemptions);
          setExcludeEmpB(_ => destringBool(p.excludeEmp));
        });
      Some(() => ReasonReactRouter.unwatchUrl(watcherId));
    });

    let onCalc = () =>
      if (areParamsDifferent(params, formParams)) {
        ReasonReactRouter.push(makeUrlParams(formParams));
      };

    let onSubmit = event => {
      ReactEvent.Mouse.preventDefault(event);
      onCalc();
    };

    let onChangeYear = event => {
      let newYearS: string = ReactEvent.Form.target(event)##value;
      let newYear = parseInt(newYearS);
      setYearS(oldYear => {
        if (oldYear != newYearS) {
          ReasonReactRouter.push(makeUrlParams({...params, year: newYear}));
        };
        newYearS;
      });
    };

    let isCalcEnabled: bool = checkParams(formParams);

    let checkSign = (valid: bool) => {
      let color = valid ? "green" : "red";
      let style = ReactDOMRe.Style.make(~marginLeft="5px", ~color, ~fontWeight="bold", ());
      <span style> {valid ? rs({js|✓|js}) : rs({js|⚠|js})} </span>;
    };

    let displayInlineBlock = ReactDOMRe.Style.make(~display="inline-block", ());
    let verticalAlignMiddle = ReactDOMRe.Style.make(~verticalAlign="middle", ());

    <>
      <h1> {rs("Taxes in NYC")} </h1>
      <div className="purpose">
        {rs("Calculate total U.S. taxes owed by an unmarried resident of NYC in ")}
        <select id="taxYearSelector" value=yearS onChange=onChangeYear>
          {ReasonReact.array(
             Array.map(
               year => <option value=year key=year> {rs(year)} </option>,
               Array.map(year => string_of_int(year), TaxRates.availableYears),
             ),
           )}
        </select>
      </div>
      <form id="input_form" method="get">
        <fieldset>
          <legend> {rs({js|⚜|js})} </legend>
          <label> {rs("Adjusted Gross Income:")} </label>
          {rs("$ ")}
          <input name="income" type_="text" size=20 value=incomeS onChange={eventS(setIncomeS)} />
          {checkSign(checkIncome(formParams.income))}
          <br />
          <label> {rs("Tax Deductions Total Amount: ")} </label>
          {rs("$ ")}
          <input name="deductions" type_="text" size=20 value=deductionsS onChange={eventS(setDeductionsS)} />
          {checkSign(checkDeductions(formParams.deductions))}
          <br />
          <label> {rs("Number of Personal Exemptions: ")} </label>
          {rs("n ")}
          <input name="exemptions" type_="number" size=20 value=exemptionsS onChange={eventS(setExemptionsS)} />
          {checkSign(checkExemptions(formParams.exemptions))}
          <br />
          <label style=displayInlineBlock>
            <input
              style=verticalAlignMiddle
              name="excludeEmp"
              type_="checkbox"
              checked={!excludeEmpB}
              onChange={event => {
                let isChecked: bool = ReactEvent.Form.target(event)##checked;
                setExcludeEmpB(_ => !isChecked);
                onCalc();
              }}
            />
            <span style=verticalAlignMiddle> {rs("Include taxes paid by employer")} </span>
          </label>
          <br />
          <input className="calc_submit" type_="submit" value="Calculate" disabled={!isCalcEnabled} onClick=onSubmit />
        </fieldset>
      </form>
      <br />
      <TaxReport params />
      <br />
      <TaxRateChart params />
      <br />
    </>;
  };
};

fixUrlIfNecessary({
  // Default parameters:
  year: TaxRates.availableYears[Array.length(TaxRates.availableYears) - 1],
  income: 100000.0,
  deductions: 0.0,
  exemptions: 1,
  excludeEmp: false,
});

ReactDOMRe.renderToElementWithId(<> <TaxAnalyzer /> </>, "root");