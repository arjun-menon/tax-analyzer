/**************
   Data Types
 **************/

type ficaTaxRate = {
  medicareTaxRate: float,
  socialSecurityTaxRate: float,
  socialSecurityWageBase: float,
};

type incomeBracket = (float, float); /* (Limit, Rate) */
type incomeTaxRateSchedule = list(incomeBracket);

type federalIncomeTaxRate = {
  rateSchedule: incomeTaxRateSchedule,
  personalExemption: float,
  standardDeduction: float,
};

type federalTaxRates = {
  income: federalIncomeTaxRate,
  fica: ficaTaxRate,
};

type nycTaxRates = {
  stateRateSchedule: incomeTaxRateSchedule,
  cityRateSchedule: incomeTaxRateSchedule,
  dependentPersonalExemption: float,
  standardDeduction: float,
};

type taxRates = {
  federal: federalTaxRates,
  nyc: nycTaxRates,
};

/**************
   Rates Data
 **************/

type taxStatus = {single: taxRates};
type taxRatesData = {year2014: taxStatus};

let getTaxRates = () : taxRatesData => {
  year2014: {
    single: {
      federal: {
        fica: {
          medicareTaxRate: 1.45,
          socialSecurityTaxRate: 6.2,
          socialSecurityWageBase: 117000.0,
        },
        income: {
          rateSchedule: [
            (9075.0, 10.0),
            (36900.0, 15.0),
            (89350.0, 25.0),
            (186350.0, 28.0),
            (405100.0, 33.0),
            (406750.0, 35.0),
            (Pervasives.max_float, 39.6),
          ],
          personalExemption: 3950.0,
          standardDeduction: 6200.0,
        },
      },
      nyc: {
        stateRateSchedule: [
          (8200.0, 4.00),
          (11300.0, 4.50),
          (13350.0, 5.25),
          (20550.0, 5.90),
          (77150.0, 6.45),
          (205850.0, 6.65),
          (1029250.0, 6.85),
          (Pervasives.max_float, 8.82),
        ],
        cityRateSchedule: [
          (12000.0, 2.907),
          (25000.0, 3.534),
          (50000.0, 3.591),
          (500000.0, 3.648),
          (Pervasives.max_float, 3.876),
        ],
        dependentPersonalExemption: 1000.0,
        standardDeduction: 7800.0,
      },
    },
  },
};
