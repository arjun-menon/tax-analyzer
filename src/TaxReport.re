type writeDetail = (string, string, string) => unit;

let rs = ReasonReact.string;

let twoPointFloatRepr: float => string = [%bs.raw
  {|
 function twoPointFloatRepr_(n) {
     return n.toFixed(2).toString();
 }
|}
];

let percent = (n: float): string => twoPointFloatRepr(n) ++ "%";

let numberWithCommas: string => string = [%bs.raw
  {|
 function numberWithCommas_(n) { /* from: http://stackoverflow.com/a/2901298 */
     return n.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ',');
 }
|}
];

/* Stringify a number representing money: */
let ns = (n: float): string => "$" ++ numberWithCommas(twoPointFloatRepr(n));
let nsi = (n: int): string => "$" ++ numberWithCommas(string_of_int(n));
let nsiof = (n: float): string => nsi(int_of_float(n));

module SlabItem = {
  [@react.component]
  let make = (~slab: TaxCalc.slabItem) =>
    <li>
      {rs(
         "Slab from "
         ++ ns(slab.fromAmt)
         ++ " to "
         ++ ns(slab.toAmt)
         ++ " = "
         ++ ns(slab.taxAmt)
         ++ " (at "
         ++ percent(slab.taxRate)
         ++ " on "
         ++ ns(slab.slabAmt)
         ++ ")",
       )}
    </li>;
};

module Slabs = {
  [@react.component]
  let make = (~slabs: array(TaxCalc.slabItem)) =>
    <ul>
      {ReasonReact.array(Array.mapi((index, slab) => <SlabItem slab key={string_of_int(index)} />, slabs))}
    </ul>;
};

module Section = {
  [@react.component]
  let make = (~label: string, ~children, ~total: float, ~pcof=?, ()) =>
    <div className="seclist">
      <p> <strong> {rs(label)} </strong> </p>
      children
      <p>
        <em> {rs(label ++ ": ")} </em>
        {rs(ns(total))}
        {switch (pcof) {
         | None => ReasonReact.null
         | Some(a) => <em> {rs(" (" ++ percent(total *. 100.0 /. a) ++ ")")} </em>
         }}
      </p>
    </div>;
};

module Point = {
  [@react.component]
  let make = (~name: string, ~value: string) => <p> {rs(name ++ ": " ++ value)} </p>;
};

module EqualsItem = {
  [@react.component]
  let make = (~label: string, ~amt: float) => <li> {rs(label ++ " = " ++ ns(amt))} </li>;
};

module MultiplicationItem = {
  [@react.component]
  let make = (~label: string, ~amt: float, ~multiplicant: int, ~one: float) =>
    <li> {rs(label ++ " = " ++ ns(amt) ++ " (" ++ string_of_int(multiplicant) ++ " x " ++ ns(one) ++ ")")} </li>;
};

module FlatRatePoint = {
  [@react.component]
  let make = (~label: string, ~tax: float, ~rate: float, ~income: float) =>
    <li> {rs(label ++ ": " ++ ns(tax) ++ " (at " ++ percent(rate) ++ " flat on " ++ ns(income) ++ ")")} </li>;
};

module CustomPoint = {
  [@react.component]
  let make = (~label: string, ~tax: float, ~explanation: string) =>
    <li> {rs(label ++ ": " ++ ns(tax))} <em> {rs(" (" ++ explanation ++ ")")} </em> </li>;
};

let calcMarginalTaxRate = (params: TaxCalc.taxParams, taxes: TaxCalc.taxesAnalysis): float => {
  let taxesWithOneExtraDollar = TaxCalc.calcTaxes({...params, income: params.income +. 1.0});
  let totalTaxDiff: float = taxesWithOneExtraDollar.totalTax -. taxes.totalTax;
  totalTaxDiff *. 100.0;
};

// let make = (~year: int, ~income: float, ~deductions: float, ~exemptions: int) => {
//   let params: TaxCalc.taxParams = {year, income, deductions, exemptions};

[@react.component]
let make = (~params: TaxCalc.taxParams) => {
  let taxes = TaxCalc.calcTaxes(params);
  let marginalTaxRate = calcMarginalTaxRate(params, taxes);

  <div className="report">
    <Point name="Adjusted Gross Income" value={ns(taxes.income)} />
    <hr />
    <h3> {rs("State & Local Income Taxes")} </h3>
    <Section label="New York Taxable Income Reductions" total={taxes.stateTaxableIncomeReductions}>
      <ul>
        <EqualsItem
          label={
            switch (taxes.stateDeduction) {
            | StateItemizedDeductions => "New York Itemized Deductions"
            | StateStandardDeduction => "New York Standard Deduction"
            }
          }
          amt={taxes.stateDeductionAmt}
        />
        {taxes.statePersonalExemptions.numOfExemptions <= 0
           ? ReasonReact.null
           : <MultiplicationItem
               label="Personal exemptions for dependents"
               amt={taxes.statePersonalExemptions.totalExemptionsAmt}
               multiplicant={taxes.statePersonalExemptions.numOfExemptions}
               one={taxes.statePersonalExemptions.oneExemptionAmt}
             />}
      </ul>
    </Section>
    <Point name="New York State Taxable Income" value={ns(taxes.stateTaxableIncome)} />
    <Section label="New York City Income Tax" total={taxes.cityIncomeTax} pcof={taxes.income}>
      <Slabs slabs={taxes.cityIncomeTaxSlabs} />
    </Section>
    <Section label="New York State Income Tax" total={taxes.stateIncomeTax} pcof={taxes.income}>
      <Slabs slabs={taxes.stateIncomeTaxSlabs} />
    </Section>
    <Point name="Total State & Local Income Taxes" value={ns(taxes.totalStateAndLocalIncomeTax)} />
    <hr />
    <h3> {rs("Federal Income Tax")} </h3>
    <Section label="Federal Taxable Income Reductions" total={taxes.federalTaxableIncomeReductions}>
      <ul>
        {switch (taxes.federalDeduction) {
         | FederalItemizedDeductions(federalItemizations) =>
           <>
             <EqualsItem
               label="State and Local Taxes Deduction"
               amt={federalItemizations.stateAndLocalTaxesDeduction}
             />
             <EqualsItem label="Other Itemized Deductions" amt={federalItemizations.otherItemizedDeductions} />
           </>
         | FederalStandardDeduction(amt) => <li> {rs("Federal Standard Deduction = " ++ ns(amt))} </li>
         }}
        {switch (taxes.federalPersonalExemptions) {
         | Some(personalExemptions) =>
           <MultiplicationItem
             label="Personal Exemptions"
             amt={personalExemptions.totalExemptionsAmt}
             multiplicant={personalExemptions.numOfExemptions}
             one={personalExemptions.oneExemptionAmt}
           />
         | None => ReasonReact.null
         }}
      </ul>
    </Section>
    <Point name="Federal Taxable Income" value={ns(taxes.federalTaxableIncome)} />
    <Section label="Federal Income Tax" total={taxes.federalIncomeTax} pcof={taxes.income}>
      <Slabs slabs={taxes.federalIncomeTaxSlabs} />
    </Section>
    <hr />
    <h3> {rs("Personal FICA & Employer Taxes")} </h3>
    <Section label="Federal Insurance Contributions Act (FICA) Tax" total={taxes.ficaTax} pcof={taxes.income}>
      <ul>
        <FlatRatePoint
          label="Social Security Old-Age, Survivors, and Disability Insurance (OASDI) Tax"
          tax={taxes.socialSecurityTax}
          rate={taxes.socialSecurityTaxRate}
          income={taxes.socialSecurityTaxableIncome}
        />
        <FlatRatePoint
          label="Medicare Hospital Insurance (HI) Tax"
          tax={taxes.medicareTax}
          rate={taxes.medicareTaxRate}
          income={taxes.income}
        />
      </ul>
    </Section>
    {params.excludeEmp
       ? ReasonReact.null
       : <Section label="Additional taxes paid by employers" total={taxes.totalEmployerTax} pcof={taxes.income}>
           <ul>
             <CustomPoint
               label="Employer's portion of the FICA Tax (see above)"
               tax={taxes.ficaTax}
               explanation="same as previous"
             />
             <CustomPoint
               label="Federal Unemployment Tax (disregarding state credits)"
               tax={taxes.federalUnemploymentTax}
               explanation={
                 percent(TaxRates.federalUnemploymentTax.federalUnemploymentTaxRate)
                 ++ " on up to "
                 ++ ns(TaxRates.federalUnemploymentTax.federalUnemploymentTaxBase)
               }
             />
             <CustomPoint
               label="New York State Disability Insurance (SDI) Tax"
               tax={taxes.stateDisabilityInsuranceTax}
               explanation={
                 ns(TaxRates.nySDI.perWeek) ++ " per week, which is " ++ ns(TaxRates.nySDI.perYear) ++ " yearly"
               }
             />
           </ul>
         </Section>}
    <hr />
    <h3>{rs("Various Totals")}</h3>
    <h4>
      {params.excludeEmp
         ? ReasonReact.null
         : <Point name="Income when including Employer-paid Taxes" value={ns(taxes.incomeInclEmployerTaxes)} />}
    </h4>
    <Point name="Total Personal Federal Income & FICA Tax (i.e. excluding employer FICA)" value={ns(taxes.totalFederalTax)} />
    <Point name="Total State & Local Taxes" value={ns(taxes.totalStateAndLocalIncomeTax)} />
    <Point name="Total Federal, State & Local Taxes" value={ns(taxes.totalPersonalTax)} />
    {params.excludeEmp
       ? ReasonReact.null : <Point name="Total Federal, State, Local & Employer Taxes" value={ns(taxes.totalTax)} />}
    <hr />
    <h3> {rs("Key Facts / Numbers")} </h3>
    <Point name="Income after Taxation" value={ns(taxes.incomeAfterTax)} />
    <Point name="Monthly Income" value={ns(taxes.incomeAfterTaxMonthly)} />
    <Point name="Effective Tax Rate" value={percent(taxes.effectiveTaxRate)} />
    <Point name="Marginal Tax Rate" value={percent(marginalTaxRate)} />
  </div>;
};