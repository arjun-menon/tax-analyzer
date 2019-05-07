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
let ns = (n: float): string =>
  "$" ++ numberWithCommas(twoPointFloatRepr(n));

let p = (n: float, percentage: float): float => n *. percentage /. 100.0;

type slabItem = {
  fromAmt: float,
  toAmt: float,
  taxAmt: float,
  taxRate: float,
  slabAmt: float,
};

let renderSlabItem = (slabItem: slabItem) =>
  ReasonReact.string(
    "Slab from "
    ++ ns(slabItem.fromAmt)
    ++ " to "
    ++ ns(slabItem.toAmt)
    ++ " = "
    ++ ns(slabItem.taxAmt)
    ++ " (at "
    ++ twoPointFloatRepr(slabItem.taxRate)
    ++ "% on "
    ++ ns(slabItem.slabAmt)
    ++ ")",
  );

let calc_slab_tax =
    (income: float, schedule: TaxRates.incomeTaxRateSchedule)
    : (float, array(slabItem)) => {
  let prev_limit = ref(0.0);
  let (totalTax, taxSlabs) =
    List.fold_left(
      (result: (float, list(slabItem)), bracket: TaxRates.incomeBracket) => {
        let (limit: float, rate: float) = bracket;

        if (income <= prev_limit^) {
          result;
        } else {
          let (tax_so_far, slabs) = result;
          let slab_amt: float =
            income <= limit ? income -. prev_limit^ : limit -. prev_limit^;
          let tax_amt: float = p(slab_amt, rate);
          let slab_limit = income > limit ? limit : income;

          let slab: slabItem = {
            fromAmt: prev_limit^,
            toAmt: slab_limit,
            taxAmt: tax_amt,
            taxRate: rate,
            slabAmt: slab_amt,
          };

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

type deductionType =
  | StandardDeduction
  | ItemizedDeductions;

type personalExemptionType = {
  oneExemptionAmt: float,
  numOfDependents: int,
  totalExemptionsAmt: float,
};

type nysTaxType = {
  nysBasicDeduction: float,
  deduction: deductionType,
  depedentExemption: personalExemptionType,
  totalDeductions: float,
  taxableIncome: float,
  nycIncomeTax: float,
  nycIncomeTaxSlabs: array(slabItem),
  stateIncomeTax: float,
  stateIncomeTaxSlabs: array(slabItem),
  totalStateAndLocalIncomeTax: float,
};

let calc_ny_taxes =
    (
      ~taxRates: TaxRates.taxRates,
      ~income: float,
      ~itemized_deductions: float,
      ~exemptions,
    )
    : nysTaxType => {
  let (nysBasicDeduction: float, deduction: deductionType) =
    itemized_deductions <= taxRates.nyc.standardDeduction
      ? (taxRates.nyc.standardDeduction, StandardDeduction)
      : (itemized_deductions, ItemizedDeductions);

  /* NYS Personal Exemptions for Dependents */
  let numOfDependents: int = Pervasives.max(exemptions - 1, 0);
  let depedentExemption: personalExemptionType = {
    oneExemptionAmt: taxRates.nyc.dependentPersonalExemption,
    numOfDependents,
    totalExemptionsAmt:
      float(numOfDependents) *. taxRates.nyc.dependentPersonalExemption,
  };

  let totalDeductions =
    nysBasicDeduction +. depedentExemption.totalExemptionsAmt;
  let taxableIncome = Pervasives.max(income -. totalDeductions, 0.0);
  let (nycIncomeTax, nycIncomeTaxSlabs) =
    calc_slab_tax(taxableIncome, taxRates.nyc.cityRateSchedule);
  let (stateIncomeTax, stateIncomeTaxSlabs) =
    calc_slab_tax(taxableIncome, taxRates.nyc.stateRateSchedule);
  let totalStateAndLocalIncomeTax = stateIncomeTax +. nycIncomeTax;

  {
    deduction,
    nysBasicDeduction,
    depedentExemption,
    totalDeductions,
    taxableIncome,
    nycIncomeTax,
    nycIncomeTaxSlabs,
    stateIncomeTax,
    stateIncomeTaxSlabs,
    totalStateAndLocalIncomeTax,
  };
};

/* ------------- Federal Taxes ------------- */

type federalItemizedDeductions = {
  stateAndLocalTaxesDeduction: float,
  otherItemizedDeductions: float,
  totalDeductions: float,
};

type federalDeductionType =
  | FederalStandardDeduction(float)
  | FederalItemizedDeductions(federalItemizedDeductions);

type federalPersonalExemptionsType =
  | None
  | Some(personalExemptionType);

type federalTaxesType = {
  federalDeduction: federalDeductionType,
  federalPersonalExemptions: federalPersonalExemptionsType,
  federalTaxableIncomeReductions: float,
  federalTaxableIncome: float,
  federalIncomeTaxSlabs: array(slabItem),
  federalIncomeTax: float,
  socialSecurityTaxableIncome: float,
  socialSecurityTaxRate: float,
  socialSecurityTax: float,
  medicareTaxRate: float,
  medicareTax: float,
  ficaTax: float,
  totalFederalTax: float,
};

let calc_federal_taxes =
    (
      ~taxRates: TaxRates.taxRates,
      ~income: float,
      ~total_state_and_local_income_tax: float,
      ~itemized_deductions: float,
      ~exemptions: int,
    )
    : federalTaxesType => {
  let salt_deduction =
    taxRates.federal.income.personalExemption === 0.0
      ? Pervasives.min(total_state_and_local_income_tax, 10000.0)
      : total_state_and_local_income_tax;

  let federal_itemized_deductions = itemized_deductions +. salt_deduction;

  let (federalDeductionAmt: float, federalDeduction: federalDeductionType) =
    federal_itemized_deductions <= taxRates.federal.income.standardDeduction
      ? (
        taxRates.federal.income.standardDeduction,
        FederalStandardDeduction(taxRates.federal.income.standardDeduction),
      )
      : (
        federal_itemized_deductions,
        FederalItemizedDeductions({
          stateAndLocalTaxesDeduction: salt_deduction,
          otherItemizedDeductions: itemized_deductions,
          totalDeductions: federal_itemized_deductions,
        }),
      );
  let personalExemptions = {
    oneExemptionAmt: taxRates.federal.income.personalExemption,
    numOfDependents: exemptions,
    totalExemptionsAmt:
      float(exemptions) *. taxRates.federal.income.personalExemption,
  };
  let federalPersonalExemptions =
    taxRates.federal.income.personalExemption > 0.0
      ? Some(personalExemptions) : None;
  let federalTaxableIncomeReductions =
    federalDeductionAmt +. personalExemptions.totalExemptionsAmt;
  let federalTaxableIncome: float =
    Pervasives.max(income -. federalTaxableIncomeReductions, 0.0);
  let (federalIncomeTax, federalIncomeTaxSlabs) =
    calc_slab_tax(federalTaxableIncome, taxRates.federal.income.rateSchedule);

  let socialSecurityTaxableIncome: float =
    Pervasives.min(income, taxRates.federal.fica.socialSecurityWageBase);
  let socialSecurityTaxRate = taxRates.federal.fica.socialSecurityTaxRate;
  let socialSecurityTax: float =
    p(socialSecurityTaxableIncome, socialSecurityTaxRate);
  let medicareTaxRate = taxRates.federal.fica.medicareTaxRate;
  let medicareTax: float = p(income, medicareTaxRate);
  let ficaTax = socialSecurityTax +. medicareTax;

  let totalFederalTax = federalIncomeTax +. ficaTax;

  {
    federalDeduction,
    federalPersonalExemptions,
    federalTaxableIncomeReductions,
    federalTaxableIncome,
    federalIncomeTax,
    federalIncomeTaxSlabs,
    socialSecurityTaxableIncome,
    socialSecurityTaxRate,
    socialSecurityTax,
    medicareTaxRate,
    medicareTax,
    ficaTax,
    totalFederalTax,
  };
};

/* ----------------------- Total Tally ----------------------- */

let flatRateItem = (label: string, tax: float, rate: float, income: float) =>
  ReasonReact.string(
    label
    ++ ": "
    ++ ns(tax)
    ++ " (at "
    ++ twoPointFloatRepr(rate)
    ++ "% flat on "
    ++ ns(income)
    ++ ")",
  );

let renderUl = (items: array(ReasonReact.reactElement)) =>
  <ul>
    {ReasonReact.array(
       Array.mapi(
         (index, item) => <li key={string_of_int(index)}> item </li>,
         items,
       ),
     )}
  </ul>;

module Slabs = {
  [@react.component]
  let make = (~slabs: array(slabItem)) =>
    renderUl(Array.map(renderSlabItem, slabs));
};

module Section = {
  [@react.component]
  let make = (~label: string, ~children, ~total: float) =>
    <div className="seclist">
      <p> <strong> {ReasonReact.string(label)} </strong> </p>
      children
      <p>
        <em> {ReasonReact.string(label ++ ": ")} </em>
        {ReasonReact.string(ns(total))}
      </p>
    </div>;
};

module Point = {
  [@react.component]
  let make = (~name: string, ~value: string) =>
    <p> {ReasonReact.string(name ++ ": " ++ value)} </p>;
};

module TaxReport = {
  [@react.component]
  let make =
      (
        ~taxRates: TaxRates.taxRates,
        ~income: float,
        ~itemizedDeductions: float,
        ~exemptions: int,
      ) => {
    let nyTaxes =
      calc_ny_taxes(
        ~taxRates,
        ~income,
        ~itemized_deductions=itemizedDeductions,
        ~exemptions,
      );
    let federalTaxes =
      calc_federal_taxes(
        ~taxRates,
        ~income,
        ~total_state_and_local_income_tax=nyTaxes.totalStateAndLocalIncomeTax,
        ~itemized_deductions=itemizedDeductions,
        ~exemptions,
      );
    let totalTax: float =
      nyTaxes.totalStateAndLocalIncomeTax +. federalTaxes.totalFederalTax;
    let incomeAfterTax: float = income -. totalTax;
    let incomeMonthly = incomeAfterTax /. 12.0;
    let effectiveTaxRate = totalTax *. 100.0 /. income;
    <>
      <Point name="Adjusted Gross Income" value={ns(income)} />
      <Section
        label="New York Taxable Income Reductions"
        total={nyTaxes.totalDeductions}>
        <ul>
          <li>
            {ReasonReact.string(
               (
                 nyTaxes.deduction == ItemizedDeductions
                   ? "Itemized Deductions"
                   : "New York State Standard Deduction"
               )
               ++ " = "
               ++ ns(nyTaxes.nysBasicDeduction),
             )}
          </li>
          {nyTaxes.depedentExemption.numOfDependents <= 0
             ? ReasonReact.null
             : <li>
                 {ReasonReact.string(
                    "Personal exemptions for dependents = "
                    ++ ns(nyTaxes.depedentExemption.totalExemptionsAmt)
                    ++ " ("
                    ++ string_of_int(
                         nyTaxes.depedentExemption.numOfDependents,
                       )
                    ++ " x "
                    ++ ns(nyTaxes.depedentExemption.oneExemptionAmt)
                    ++ ")",
                  )}
               </li>}
        </ul>
      </Section>
      <Point
        name="New York State Taxable Income"
        value={ns(nyTaxes.taxableIncome)}
      />
      <Section label="New York City Income Tax" total={nyTaxes.nycIncomeTax}>
        <Slabs slabs={nyTaxes.nycIncomeTaxSlabs} />
      </Section>
      <Section
        label="New York State Income Tax" total={nyTaxes.stateIncomeTax}>
        <Slabs slabs={nyTaxes.stateIncomeTaxSlabs} />
      </Section>
      <Point
        name="Total New York State & City Taxes"
        value={ns(nyTaxes.totalStateAndLocalIncomeTax)}
      />
      <Section
        label="Federal Taxable Income Reductions"
        total={federalTaxes.federalTaxableIncomeReductions}>
        <ul>
          {switch (federalTaxes.federalDeduction) {
           | FederalItemizedDeductions(federalItemizations) =>
             <>
               <li>
                 {ReasonReact.string(
                    "State and Local Taxes Deduction = "
                    ++ ns(federalItemizations.stateAndLocalTaxesDeduction),
                  )}
               </li>
               <li>
                 {ReasonReact.string(
                    "Other Itemized Deductions = "
                    ++ ns(federalItemizations.otherItemizedDeductions),
                  )}
               </li>
             </>
           | FederalStandardDeduction(amt) =>
             <li>
               {ReasonReact.string(
                  "Federal Standard Deduction = " ++ ns(amt),
                )}
             </li>
           }}
          {switch (federalTaxes.federalPersonalExemptions) {
           | Some(personalExemptions) =>
             <li>
               {ReasonReact.string(
                  "Personal Exemptions = "
                  ++ ns(personalExemptions.totalExemptionsAmt)
                  ++ "  ("
                  ++ string_of_int(personalExemptions.numOfDependents)
                  ++ " x "
                  ++ ns(personalExemptions.oneExemptionAmt)
                  ++ ")",
                )}
             </li>
           | None => ReasonReact.null
           }}
        </ul>
      </Section>
      <Point
        name="Federal Taxable Income"
        value={ns(federalTaxes.federalTaxableIncome)}
      />
      <Section
        label="Federal Income Tax" total={federalTaxes.federalIncomeTax}>
        <Slabs slabs={federalTaxes.federalIncomeTaxSlabs} />
      </Section>
      <Section
        label="Federal Insurance Contributions Act (FICA) Tax"
        total={federalTaxes.ficaTax}>
        {renderUl([|
           flatRateItem(
             "Social Security Old-Age, Survivors, and Disability Insurance (OASDI) Tax",
             federalTaxes.socialSecurityTax,
             federalTaxes.socialSecurityTaxRate,
             federalTaxes.socialSecurityTaxableIncome,
           ),
           flatRateItem(
             "Medicare Hospital Insurance (HI) Tax",
             federalTaxes.medicareTax,
             federalTaxes.medicareTaxRate,
             income,
           ),
         |])}
      </Section>
      <Point
        name="Total Federal Taxes"
        value={ns(federalTaxes.totalFederalTax)}
      />
      <Point name="Total Federal, State & Local Taxes" value={ns(totalTax)} />
      <hr />
      <Point name="Income after Taxation" value={ns(incomeAfterTax)} />
      <Point name="Monthly Income" value={ns(incomeMonthly)} />
      <Point
        name="Effective Tax Rate"
        value={twoPointFloatRepr(effectiveTaxRate) ++ "%"}
      />
    </>;
  };
};

let renderReport =
    (
      taxRates: TaxRates.taxRates,
      income: float,
      itemizedDeductions: float,
      exemptions: int,
    ) => {
  ReactDOMRe.renderToElementWithId(
    <TaxReport taxRates income itemizedDeductions exemptions />,
    "report",
  );
};