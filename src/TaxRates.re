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

type federalUnemploymentTax = {
  federalUnemploymentTaxBase: float,
  federalUnemploymentTaxRate: float,
};

type federalIncomeTaxRate = {
  rateSchedule: incomeTaxRateSchedule,
  personalExemption: float,
  standardDeduction: float,
};

type federalTaxRates = {
  income: federalIncomeTaxRate,
  fica: ficaTaxRate,
  federalUnemploymentTax,
};

type nySDI = {
  perWeek: float,
  perYear: float,
};

type nycTaxRates = {
  stateRateSchedule: incomeTaxRateSchedule,
  cityRateSchedule: incomeTaxRateSchedule,
  dependentPersonalExemption: float,
  standardDeduction: float,
  nySDI,
};

type taxRates = {
  federal: federalTaxRates,
  nyc: nycTaxRates,
};

/**************
   Rates Data
 **************/
let federalUnemploymentTax: federalUnemploymentTax = {
  // These have not changed in a while.
  federalUnemploymentTaxBase: 7000.0,
  federalUnemploymentTaxRate: 6.0,
};

let nySDI: nySDI = {
  // Has not changed much either.
  perWeek: 0.60,
  perYear: 0.60 *. 52.0,
};

module IntMap = Map.Make({
  type t = int;
  let compare = compare
});

let singleTaxRatesByYear: Belt.Map.Int.t(taxRates) =
  Belt.Map.Int.fromArray([|
    // Year 2014 Single Tax Rates
    (
      2014,
      {
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
              (Float.infinity, 39.6),
            ],
            personalExemption: 3950.0,
            standardDeduction: 6200.0,
          },
          federalUnemploymentTax,
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
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [
            (12000.0, 2.907),
            (25000.0, 3.534),
            (50000.0, 3.591),
            (500000.0, 3.648),
            (Float.infinity, 3.876),
          ],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 7800.0,
          nySDI,
        },
      },
    ),
    // Year 2015 Single Tax Rates
    (
      2015,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 118500.0,
          },
          income: {
            rateSchedule: [
              (9225.0, 10.0),
              (37450.0, 15.0),
              (90750.0, 25.0),
              (189300.0, 28.0),
              (411500.0, 33.0),
              (413200.0, 35.0),
              (Float.infinity, 39.6),
            ],
            personalExemption: 4000.0,
            standardDeduction: 6300.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8400.0, 4.00),
            (11600.0, 4.50),
            (13750.0, 5.25),
            (21150.0, 5.90),
            (79600.0, 6.45),
            (212500.0, 6.65),
            (1062650.0, 6.85),
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [
            (12000.0, 2.907),
            (25000.0, 3.534),
            (50000.0, 3.591),
            (500000.0, 3.648),
            (Float.infinity, 3.876),
          ],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 7900.0,
          nySDI,
        },
      },
    ),
    // Year 2016 Single Tax Rates
    (
      2016,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 118500.0,
          },
          income: {
            rateSchedule: [
              (9275.0, 10.0),
              (37650.0, 15.0),
              (91150.0, 25.0),
              (190150.0, 28.0),
              (413350.0, 33.0),
              (415050.0, 35.0),
              (Float.infinity, 39.6),
            ],
            personalExemption: 4050.0,
            standardDeduction: 6300.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8450.0, 4.00),
            (11650.0, 4.50),
            (13850.0, 5.25),
            (21300.0, 5.90),
            (80150.0, 6.45),
            (214000.0, 6.65),
            (1070350.0, 6.85),
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [
            (12000.0, 2.907),
            (25000.0, 3.534),
            (50000.0, 3.591),
            (500000.0, 3.648),
            (Float.infinity, 3.876),
          ],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 7950.0,
          nySDI,
        },
      },
    ),
    // Year 2017 Single Tax Rates
    (
      2017,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 127200.0,
          },
          income: {
            rateSchedule: [
              (9325.0, 10.0),
              (37950.0, 15.0),
              (91900.0, 25.0),
              (191650.0, 28.0),
              (416700.0, 33.0),
              (418400.0, 35.0),
              (Float.infinity, 39.6),
            ],
            personalExemption: 4050.0,
            standardDeduction: 6350.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8500.0, 4.00),
            (11700.0, 4.50),
            (13900.0, 5.25),
            (21400.0, 5.90),
            (80650.0, 6.45),
            (215400.0, 6.65),
            (1077550.0, 6.85),
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Float.infinity, 3.876)],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 8000.0,
          nySDI,
        },
      },
    ),
    // Year 2018 Single Tax Rates
    (
      2018,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 128400.0,
          },
          income: {
            rateSchedule: [
              (9525.0, 10.0),
              (38700.0, 12.0),
              (82500.0, 22.0),
              (157500.0, 24.0),
              (200000.0, 32.0),
              (500000.0, 35.0),
              (Float.infinity, 37.0),
            ],
            personalExemption: 0.0,
            standardDeduction: 12000.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8500.0, 4.00),
            (11700.0, 4.50),
            (13900.0, 5.25),
            (21400.0, 5.90),
            (80650.0, 6.45),
            (215400.0, 6.65),
            (1077550.0, 6.85),
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Float.infinity, 3.876)],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 8000.0,
          nySDI,
        },
      },
    ),
    // Year 2019 Single Tax Rates
    (
      2019,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 132900.0,
          },
          income: {
            rateSchedule: [
              (9700.0, 10.0),
              (39475.0, 12.0),
              (84200.0, 22.0),
              (160725.0, 24.0),
              (204100.0, 32.0),
              (510300.0, 35.0),
              (Float.infinity, 37.0),
            ],
            personalExemption: 0.0,
            standardDeduction: 12200.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8500.0, 4.00),
            (11700.0, 4.50),
            (13900.0, 5.25),
            (21400.0, 5.90),
            (80650.0, 6.45),
            (215400.0, 6.65),
            (1077550.0, 6.85),
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Float.infinity, 3.876)],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 8000.0,
          nySDI,
        },
      },
    ),
    // Year 2020 Single Tax Rates
    (
      2020,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 137700.0,
          },
          income: {
            rateSchedule: [
              (9875.0, 10.0),
              (40125.0, 12.0),
              (85525.0, 22.0),
              (163300.0, 24.0),
              (207350.0, 32.0),
              (518400.0, 35.0),
              (Float.infinity, 37.0),
            ],
            personalExemption: 0.0,
            standardDeduction: 12400.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8500.0, 4.00),
            (11700.0, 4.50),
            (13900.0, 5.25),
            (21400.0, 5.90),
            (80650.0, 6.45),
            (215400.0, 6.65),
            (1077550.0, 6.85),
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Float.infinity, 3.876)],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 8000.0,
          nySDI,
        },
      },
    ),
    // Year 2021 Single Tax Rates
    (
      2021,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 142800.0,
          },
          income: {
            rateSchedule: [
              (9950.0, 10.0),
              (40525.0, 12.0),
              (86375.0, 22.0),
              (164925.0, 24.0),
              (209425.0, 32.0),
              (523600.0, 35.0),
              (Float.infinity, 37.0),
            ],
            personalExemption: 0.0,
            standardDeduction: 12550.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8500.0, 4.00),
            (11700.0, 4.50),
            (13900.0, 5.25),
            (21400.0, 5.90),
            (80650.0, 6.45),
            (215400.0, 6.65),
            (1077550.0, 6.85),
            (Float.infinity, 8.82),
          ],
          cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Float.infinity, 3.876)],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 8000.0,
          nySDI,
        },
      },
    ),
    // Year 2022 Single Tax Rates
    (
      2022,
      {
        federal: {
          fica: {
            medicareTaxRate: 1.45,
            socialSecurityTaxRate: 6.2,
            socialSecurityWageBase: 147000.0,
          },
          income: {
            rateSchedule: [
              (10275.0, 10.0),
              (41775.0, 12.0),
              (89075.0, 22.0),
              (170050.0, 24.0),
              (215950.0, 32.0),
              (539900.0, 35.0),
              (Float.infinity, 37.0),
            ],
            personalExemption: 0.0,
            standardDeduction: 12950.0,
          },
          federalUnemploymentTax,
        },
        nyc: {
          stateRateSchedule: [
            (8500.0, 4.00),
            (11700.0, 4.50),
            (13900.0, 5.25),
            (21400.0, 5.90),
            (80650.0, 6.33),
            (215400.0, 6.57),
            (1077550.0, 6.85),
            (Float.infinity, 10.90),
          ],
          cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Float.infinity, 3.876)],
          dependentPersonalExemption: 1000.0,
          standardDeduction: 8000.0,
          nySDI,
        },
      },
    ),
    // Year 2023 Single Tax Rates
    // Year 2024 Single Tax Rates
    // Year 2025 Single Tax Rates
  |]);

let getSingleRatesForYear = (year: int): taxRates => Belt.Map.Int.getExn(singleTaxRatesByYear, year);
let availableYears: array(int) = Belt.Map.Int.keysToArray(singleTaxRatesByYear);
