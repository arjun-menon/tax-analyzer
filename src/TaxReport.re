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
let ns = (n: float): string => "$" ++ numberWithCommas(twoPointFloatRepr(n));

let renderSlabItem = (slabItem: TaxCalc.slabItem) =>
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

let renderUl = (items: array(ReasonReact.reactElement)) =>
  <ul> {ReasonReact.array(Array.mapi((index, item) => <li key={string_of_int(index)}> item </li>, items))} </ul>;

module Slabs = {
  [@react.component]
  let make = (~slabs: array(TaxCalc.slabItem)) => renderUl(Array.map(renderSlabItem, slabs));
};

module Section = {
  [@react.component]
  let make = (~label: string, ~children, ~total: float) =>
    <div className="seclist">
      <p> <strong> {ReasonReact.string(label)} </strong> </p>
      children
      <p> <em> {ReasonReact.string(label ++ ": ")} </em> {ReasonReact.string(ns(total))} </p>
    </div>;
};

module Point = {
  [@react.component]
  let make = (~name: string, ~value: string) => <p> {ReasonReact.string(name ++ ": " ++ value)} </p>;
};

module EqualsPoint = {
  [@react.component]
  let make = (~label: string, ~amt: float) => ReasonReact.string(label ++ " = " ++ ns(amt));
};

module MultiplicationPoint = {
  [@react.component]
  let make = (~label: string, ~amt: float, ~multiplicant: int, ~one: float) =>
    ReasonReact.string(
      label ++ " = " ++ ns(amt) ++ " (" ++ string_of_int(multiplicant) ++ " x " ++ ns(one) ++ ")",
    );
};

module FlatRatePoint = {
  [@react.component]
  let make = (~label: string, ~tax: float, ~rate: float, ~income: float) =>
    ReasonReact.string(
      label ++ ": " ++ ns(tax) ++ " (at " ++ twoPointFloatRepr(rate) ++ "% flat on " ++ ns(income) ++ ")",
    );
};

[@react.component]
let make = (~taxRates: TaxRates.taxRates, ~income: float, ~itemizedDeductions: float, ~exemptions: int) => {
  let taxes = TaxCalc.calcTaxes(~taxRates, ~income, ~itemizedDeductions, ~exemptions);
  <>
    <Point name="Adjusted Gross Income" value={ns(income)} />
    <Section label="New York Taxable Income Reductions" total={taxes.stateTaxableIncomeReductions}>
      <ul>
        <li>
          <EqualsPoint
            label={
              switch (taxes.stateDeduction) {
              | StateItemizedDeductions => "New York Itemized Deductions"
              | StateStandardDeduction => "New York Standard Deduction"
              }
            }
            amt={taxes.stateDeductionAmt}
          />
        </li>
        {taxes.statePersonalExemptions.numOfExemptions <= 0
           ? ReasonReact.null
           : <li>
               <MultiplicationPoint
                 label="Personal exemptions for dependents"
                 amt={taxes.statePersonalExemptions.totalExemptionsAmt}
                 multiplicant={taxes.statePersonalExemptions.numOfExemptions}
                 one={taxes.statePersonalExemptions.oneExemptionAmt}
               />
             </li>}
      </ul>
    </Section>
    <Point name="New York State Taxable Income" value={ns(taxes.stateTaxableIncome)} />
    <Section label="New York City Income Tax" total={taxes.cityIncomeTax}>
      <Slabs slabs={taxes.cityIncomeTaxSlabs} />
    </Section>
    <Section label="New York State Income Tax" total={taxes.stateIncomeTax}>
      <Slabs slabs={taxes.stateIncomeTaxSlabs} />
    </Section>
    <Point name="Total New York State & City Taxes" value={ns(taxes.totalStateAndLocalIncomeTax)} />
    <Section label="Federal Taxable Income Reductions" total={taxes.federalTaxableIncomeReductions}>
      <ul>
        {switch (taxes.federalDeduction) {
         | FederalItemizedDeductions(federalItemizations) =>
           <>
             <li>
               <EqualsPoint
                 label="State and Local Taxes Deduction"
                 amt={federalItemizations.stateAndLocalTaxesDeduction}
               />
             </li>
             <li>
               <EqualsPoint label="Other Itemized Deductions" amt={federalItemizations.otherItemizedDeductions} />
             </li>
           </>
         | FederalStandardDeduction(amt) =>
           <li> {ReasonReact.string("Federal Standard Deduction = " ++ ns(amt))} </li>
         }}
        {switch (taxes.federalPersonalExemptions) {
         | Some(personalExemptions) =>
           <li>
             <MultiplicationPoint
               label="Personal Exemptions"
               amt={personalExemptions.totalExemptionsAmt}
               multiplicant={personalExemptions.numOfExemptions}
               one={personalExemptions.oneExemptionAmt}
             />
           </li>
         | None => ReasonReact.null
         }}
      </ul>
    </Section>
    <Point name="Federal Taxable Income" value={ns(taxes.federalTaxableIncome)} />
    <Section label="Federal Income Tax" total={taxes.federalIncomeTax}>
      <Slabs slabs={taxes.federalIncomeTaxSlabs} />
    </Section>
    <Section label="Federal Insurance Contributions Act (FICA) Tax" total={taxes.ficaTax}>
      {renderUl([|
         <FlatRatePoint
           label="Social Security Old-Age, Survivors, and Disability Insurance (OASDI) Tax"
           tax={taxes.socialSecurityTax}
           rate={taxes.socialSecurityTaxRate}
           income={taxes.socialSecurityTaxableIncome}
         />,
         <FlatRatePoint
           label="Medicare Hospital Insurance (HI) Tax"
           tax={taxes.medicareTax}
           rate={taxes.medicareTaxRate}
           income
         />,
       |])}
    </Section>
    <Point name="Total Federal Taxes" value={ns(taxes.totalFederalTax)} />
    <Point name="Total Federal, State & Local Taxes" value={ns(taxes.totalTax)} />
    <hr />
    <Point name="Income after Taxation" value={ns(taxes.incomeAfterTax)} />
    <Point name="Monthly Income" value={ns(taxes.incomeAfterTaxMonthly)} />
    <Point name="Effective Tax Rate" value={twoPointFloatRepr(taxes.effectiveTaxRate) ++ "%"} />
  </>;
};