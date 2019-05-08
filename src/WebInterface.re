let renderReport = (taxYear: int, income: float, itemizedDeductions: float, exemptions: int) => {
  let taxRates: TaxRates.taxRates = TaxRates.getTaxRates(taxYear);
  let taxes: TaxCalc.taxesAnalysis = TaxCalc.calcTaxes(~taxRates, ~income, ~itemizedDeductions, ~exemptions);
  ReactDOMRe.renderToElementWithId(<TaxReport taxes />, "report");
};