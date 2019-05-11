let renderReport = (taxYear: int, income: float, itemizedDeductions: float, exemptions: int) => {
  let taxRates: TaxRates.taxRates = TaxRates.getTaxRates(taxYear);
  let taxes: TaxCalc.taxesAnalysis = TaxCalc.calcTaxes(~taxRates, ~income, ~itemizedDeductions, ~exemptions);
  let root = <>
    <br />
    <TaxReport taxes />
    <br />
  </>;
  ReactDOMRe.renderToElementWithId(root, "root");
};
let renderError = (message: string) => {
  ReactDOMRe.renderToElementWithId(<span> {ReasonReact.string(message)} </span>, "report");
};