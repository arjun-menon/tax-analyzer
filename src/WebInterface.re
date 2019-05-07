let renderReport = (taxYear: int, income: float, itemizedDeductions: float, exemptions: int) => {
  let taxRates: TaxRates.taxRates = TaxRates.getTaxRates(taxYear);
  ReactDOMRe.renderToElementWithId(<TaxReport taxRates income itemizedDeductions exemptions />, "report");
};