type writeDetail = (string, string, string) => unit;

let twoPointFloatRepr: float => string = [%bs.raw
  {|
 function twoPointFloatRepr_(n) {
     return n.toFixed(2).toString();
 }
|}
];

let numberWithCommas: string => string = [%bs.raw
  {|
 function numberWithCommas_(n) { /* from: http://stackoverflow.com/a/2901298 */
     return n.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ',');
 }
|}
];

/* Stringify a number representing money: */
let ns = (n: float) : string => "$" ++ numberWithCommas(twoPointFloatRepr(n));

let p = (n: float, percentage: float) : float => n *. percentage /. 100.0;

let calc_slab_tax = (income: float, schedule: TaxRates.incomeTaxRateSchedule, w: writeDetail) : float => {
  let prev_limit = ref(0.0);
  List.fold_left(
    (result: float, bracket: TaxRates.incomeBracket) => {
      let (limit: float, rate: float) = bracket;

      if (income <= prev_limit^) {
        result;
      } else {
        let slab_amt: float = income <= limit ? income -. prev_limit^ : limit -. prev_limit^;

        let tax_amt: float = p(slab_amt, rate);

        let slab_limit = income > limit ? ns(limit) : ns(income);
        w(
          "b",
          "Slab from "
          ++ ns(prev_limit^)
          ++ " to "
          ++ slab_limit
          ++ " = "
          ++ ns(tax_amt)
          ++ " (at "
          ++ twoPointFloatRepr(rate)
          ++ "% on "
          ++ ns(slab_amt)
          ++ ")",
          "",
        );

        prev_limit := limit;
        result +. tax_amt;
      };
    },
    0.0,
    schedule,
  );
};

type slabItem = {
  fromAmt: float,
  toAmt: float,
  taxAmt: float,
  taxRate: float,
  slabAmt: float
};

let renderSlabItem = (slabItem: slabItem) =>
  ReasonReact.string(
    "Slab from " ++ ns(slabItem.fromAmt)
    ++ " to " ++ ns(slabItem.toAmt)
    ++ " = " ++ ns(slabItem.taxAmt)
    ++ " (at " ++ twoPointFloatRepr(slabItem.taxRate)
    ++ "% on " ++ ns(slabItem.slabAmt) ++ ")");

let calc_slab_tax_ = (income: float, schedule: TaxRates.incomeTaxRateSchedule) : (float, array(slabItem)) => {
  let prev_limit = ref(0.0);
  let (totalTax, taxSlabs) = List.fold_left(
    (result: (float, list(slabItem)), bracket: TaxRates.incomeBracket) => {
      let (limit: float, rate: float) = bracket;

      if (income <= prev_limit^) {
        result;
      } else {
        let (tax_so_far, slabs) = result;
        let slab_amt: float = income <= limit ? income -. prev_limit^ : limit -. prev_limit^;
        let tax_amt: float = p(slab_amt, rate);
        let slab_limit = income > limit ? limit : income;

        let slab: slabItem = {
          fromAmt: prev_limit^,
          toAmt: slab_limit,
          taxAmt: tax_amt,
          taxRate: rate,
          slabAmt: slab_amt
        }

        prev_limit := limit;
        let accumulated_tax = tax_so_far +. tax_amt;

        (accumulated_tax, [slab, ...slabs]);
      };
    },
    (0.0, []),
    schedule,
  );

  (totalTax, Array.of_list(List.rev(taxSlabs)));
};

/* ------------- New York State & City Taxes ------------- */

let calc_ny_taxes =
    (~taxRates: TaxRates.taxRates, ~income: float, ~itemized_deductions: float, ~exemptions: int, ~w: writeDetail)
    : float => {
  w("l_start", "New York State Tax Deductions", "");

  let nys_basic_deduction: float =
    if (itemized_deductions <= taxRates.nyc.standardDeduction) {
      w("b", "New York State Standard Deduction = " ++ ns(taxRates.nyc.standardDeduction), "");
      taxRates.nyc.standardDeduction;
    } else {
      w("b", "Itemized Deductions = " ++ ns(itemized_deductions), "");
      itemized_deductions;
    };

  /* NYS Personal Exemptions for Dependents */
  let no_of_dependents: int = Pervasives.max(exemptions - 1, 0);

  let nys_personal_exemptions: float = float(no_of_dependents) *. taxRates.nyc.dependentPersonalExemption;

  if (no_of_dependents > 0) {
    w(
      "b",
      "Personal exemptions for dependents = "
      ++ ns(nys_personal_exemptions)
      ++ "  ("
      ++ string_of_int(no_of_dependents)
      ++ " x "
      ++ ns(taxRates.nyc.dependentPersonalExemption)
      ++ ")",
      "",
    );
  };

  let nys_deduction = nys_basic_deduction +. nys_personal_exemptions;

  w("l_end", "New York State Tax Deductions", ns(nys_deduction));

  let nys_taxable_income = Pervasives.max(income -. nys_deduction, 0.0);

  w("a", "New York State Taxable Income", ns(nys_taxable_income));

  w("l_start", "New York City Income Tax", "");

  let nyc_income_tax: float = calc_slab_tax(nys_taxable_income, taxRates.nyc.cityRateSchedule, w);

  w("l_end", "New York City Income Tax", ns(nyc_income_tax));

  w("l_start", "New York State Income Tax", "");

  let nys_income_tax: float = calc_slab_tax(nys_taxable_income, taxRates.nyc.stateRateSchedule, w);

  w("l_end", "New York State Income Tax", ns(nys_income_tax));

  let total_state_and_local_income_tax = nys_income_tax +. nyc_income_tax;

  w("a", "Total New York State & City Taxes", ns(total_state_and_local_income_tax));

  total_state_and_local_income_tax;
};

type deductionType = StandardDeduction | ItemizedDeductions;

type nysDepedentExemptionType = {
  oneExemptionAmt: float,
  numOfDependents: int,
  totalExemptionsAmt: float,
};

type nysTaxType = {
  nysBasicDeduction: float,
  deduction: deductionType,
  depedentExemption: nysDepedentExemptionType,
  totalDeductions: float,
  taxableIncome: float,
  nycIncomeTax: float,
  nycIncomeTaxSlabs: array(slabItem),
  stateIncomeTax: float,
  stateIncomeTaxSlabs: array(slabItem),
  totalStateAndLocalIncomeTax: float
};

let calc_ny_taxes_ =
    (~taxRates: TaxRates.taxRates, ~income: float, ~itemized_deductions: float, ~exemptions)
    : nysTaxType => {
  let (nysBasicDeduction: float, deduction: deductionType) =
    (itemized_deductions <= taxRates.nyc.standardDeduction) ?
      (taxRates.nyc.standardDeduction, StandardDeduction)
    : (itemized_deductions, ItemizedDeductions);

  /* NYS Personal Exemptions for Dependents */
  let numOfDependents: int = Pervasives.max(exemptions - 1, 0);
  let depedentExemption: nysDepedentExemptionType = {
    oneExemptionAmt: taxRates.nyc.dependentPersonalExemption,
    numOfDependents: numOfDependents,
    totalExemptionsAmt: float(numOfDependents) *. taxRates.nyc.dependentPersonalExemption,
  }

  let totalDeductions = nysBasicDeduction +. depedentExemption.totalExemptionsAmt;
  let taxableIncome = Pervasives.max(income -. totalDeductions, 0.0);
  let (nycIncomeTax, nycIncomeTaxSlabs) = calc_slab_tax_(taxableIncome, taxRates.nyc.cityRateSchedule);
  let (stateIncomeTax, stateIncomeTaxSlabs) = calc_slab_tax_(taxableIncome, taxRates.nyc.stateRateSchedule);
  let totalStateAndLocalIncomeTax = stateIncomeTax +. nycIncomeTax;

  {deduction, nysBasicDeduction, depedentExemption, totalDeductions, taxableIncome,
  nycIncomeTax, nycIncomeTaxSlabs, stateIncomeTax, stateIncomeTaxSlabs, totalStateAndLocalIncomeTax}
};

/* ------------- Federal Taxes ------------- */

let calc_federal_taxes =
    (
      ~taxRates: TaxRates.taxRates,
      ~income: float,
      ~total_state_and_local_income_tax: float,
      ~itemized_deductions: float,
      ~exemptions: int,
      ~w: writeDetail,
    )
    : float => {
  w("l_start", "Federal Tax Deductions", "");

  let salt_deduction =
    taxRates.federal.income.personalExemption === 0.0 ?
      Pervasives.min(total_state_and_local_income_tax, 10000.0) : total_state_and_local_income_tax;

  let federal_itemized_deductions = itemized_deductions +. salt_deduction;

  let federal_basic_deduction =
    if (federal_itemized_deductions > taxRates.federal.income.standardDeduction) {
      w("b", "State and Local Taxes Deduction = " ++ ns(salt_deduction), "");
      w("b", "Additional Itemized Deductions = " ++ ns(itemized_deductions), "");
      federal_itemized_deductions;
    } else {
      w("b", "Federal Standard Deduction = " ++ ns(taxRates.federal.income.standardDeduction), "");
      taxRates.federal.income.standardDeduction;
    };

  let federal_personal_exemptions = float(exemptions) *. taxRates.federal.income.personalExemption;

  w(
    "b",
    "Personal Exemptions = "
    ++ ns(federal_personal_exemptions)
    ++ "  ("
    ++ string_of_int(exemptions)
    ++ " x "
    ++ ns(taxRates.federal.income.personalExemption)
    ++ ")",
    "",
  );

  let federal_deduction = federal_basic_deduction +. federal_personal_exemptions;

  w("l_end", "Federal Tax Deductions", ns(federal_deduction));

  let federal_taxable_income: float = Pervasives.max(income -. federal_deduction, 0.0);

  w("a", "Federal Taxable Income", ns(federal_taxable_income));

  w("l_start", "Federal Income Tax", "");

  let federal_income_tax: float = calc_slab_tax(federal_taxable_income, taxRates.federal.income.rateSchedule, w);

  w("l_end", "Federal Income Tax", ns(federal_income_tax));

  w("l_start", "Federal Insurance Contributions Act (FICA) Tax", "");

  let social_security_taxable_income: float = Pervasives.min(income, taxRates.federal.fica.socialSecurityWageBase);
  let social_security_tax: float = p(social_security_taxable_income, taxRates.federal.fica.socialSecurityTaxRate);

  w(
    "b",
    "Social Security Old-Age, Survivors, and Disability Insurance (OASDI) Tax: "
    ++ ns(social_security_tax)
    ++ " (at "
    ++ twoPointFloatRepr(taxRates.federal.fica.socialSecurityTaxRate)
    ++ "% flat on "
    ++ ns(social_security_taxable_income)
    ++ ")",
    "",
  );

  let medicare_tax: float = p(income, taxRates.federal.fica.medicareTaxRate);

  w(
    "b",
    "Medicare Hospital Insurance (HI) Tax: "
    ++ ns(medicare_tax)
    ++ " (at "
    ++ twoPointFloatRepr(taxRates.federal.fica.medicareTaxRate)
    ++ "% flat on "
    ++ ns(income)
    ++ ")",
    "",
  );

  let fica_tax = social_security_tax +. medicare_tax;

  w("l_end", "Federal Insurance Contributions Act (FICA) Tax", ns(fica_tax));

  let total_federal_tax = federal_income_tax +. fica_tax;

  w("a", "Total Federal Taxes", ns(total_federal_tax));

  total_federal_tax;
};

/* ----------------------- Total Tally ----------------------- */

let renderUl = (items: array(ReasonReact.reactElement)) =>
  <ul>
    {ReasonReact.array(Array.map(item => <li>item</li>, items))}
  </ul>;

let renderSlabs = (slabs: array(slabItem)) =>
  renderUl(Array.map(renderSlabItem, slabs))

module Section = {
  [@react.component]
  let make = (~label: string, ~children, ~total: float) =>
    <div className="seclist">
      <p>
        <strong>{ReasonReact.string(label)}</strong>
        children
        <em>{ReasonReact.string(label ++ ": ")}</em>
        {ReasonReact.string(ns(total))}
      </p>
    </div>;
};

module Point = {
  [@react.component]
  let make = (~name: string, ~value: string) =>
    <p>{ReasonReact.string(name ++ ": " ++ value)}</p>;
};

let calc_taxes_ =
    (taxRates: TaxRates.taxRates, income: float, itemized_deductions: float, exemptions: int) => {
  let nyTaxes = calc_ny_taxes_(~taxRates, ~income, ~itemized_deductions, ~exemptions);

  let report = <div>
    <p>{ReasonReact.string("Adjusted Gross Income: " ++ ns(income))}</p>
    <Section label="New York State Tax Deductions" total={nyTaxes.totalDeductions}>
      <ul>
      <li>{ReasonReact.string((nyTaxes.deduction == ItemizedDeductions
        ? "Itemized Deductions" : "New York State Standard Deduction")
        ++ " = " ++ ns(nyTaxes.nysBasicDeduction))}</li>
      {nyTaxes.depedentExemption.numOfDependents <= 0 ? ReasonReact.null :
        <li>{ReasonReact.string(
          "Personal exemptions for dependents = "
          ++ ns(nyTaxes.depedentExemption.totalExemptionsAmt)
          ++ " (" ++ string_of_int(nyTaxes.depedentExemption.numOfDependents)
          ++ " x " ++ ns(nyTaxes.depedentExemption.oneExemptionAmt) ++ ")"
        )}</li>}
      </ul>
    </Section>
    <Point name="New York State Taxable Income" value={ns(nyTaxes.taxableIncome)} />
    <Section label="New York City Income Tax" total={nyTaxes.nycIncomeTax}>
      {renderSlabs(nyTaxes.nycIncomeTaxSlabs)}
    </Section>
    <Section label="New York State Income Tax" total={nyTaxes.stateIncomeTax}>
      {renderSlabs(nyTaxes.stateIncomeTaxSlabs)}
    </Section>
    <Point name="Total New York State & City Taxes" value={ns(nyTaxes.totalStateAndLocalIncomeTax)} />
  </div>;
  ReactDOMRe.renderToElementWithId(report, "report");
};

let calc_taxes =
    (taxRates: TaxRates.taxRates, income: float, itemized_deductions: float, exemptions: int, w: writeDetail)
    : float => {
  w("a", "Adjusted Gross Income", ns(income));

  let total_state_and_local_income_tax = calc_ny_taxes(~taxRates, ~income, ~itemized_deductions, ~exemptions, ~w);
  let total_federal_tax =
    calc_federal_taxes(~taxRates, ~income, ~total_state_and_local_income_tax, ~itemized_deductions, ~exemptions, ~w);

  let total_tax: float = total_federal_tax +. total_state_and_local_income_tax;

  w("a", "Total Federal, State & Local Taxes", ns(total_tax));

  w("x", "<hr/>", "");

  let income_after_tax: float = income -. total_tax;

  let monthly_income_after_tax = income_after_tax /. 12.0;

  w("a", "Income after Taxation", ns(income_after_tax));

  w("a", "Effective Tax Rate", twoPointFloatRepr(total_tax *. 100.0 /. income) ++ "%");

  w("a", "Monthly Income", ns(monthly_income_after_tax));

  total_tax;
};
