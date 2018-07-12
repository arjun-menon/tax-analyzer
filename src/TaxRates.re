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

let getTaxRatesForYear2014Single = () : taxRates => {
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
};

let getTaxRatesForYear2015Single = () : taxRates => {
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
        (Pervasives.max_float, 39.6),
      ],
      personalExemption: 4000.0,
      standardDeduction: 6300.0,
    },
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
    standardDeduction: 7900.0,
  },
};

let getTaxRatesForYear2016Single = () : taxRates => {
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
        (Pervasives.max_float, 39.6),
      ],
      personalExemption: 4050.0,
      standardDeduction: 6300.0,
    },
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
    standardDeduction: 7950.0,
  },
};

let getTaxRatesForYear2017Single = () : taxRates => {
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
        (Pervasives.max_float, 39.6),
      ],
      personalExemption: 4050.0,
      standardDeduction: 6350.0,
    },
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
      (Pervasives.max_float, 8.82),
    ],
    cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Pervasives.max_float, 3.876)],
    dependentPersonalExemption: 1000.0,
    standardDeduction: 8000.0,
  },
};

let getTaxRatesForYear2018Single = () : taxRates => {
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
        (Pervasives.max_float, 37.0),
      ],
      personalExemption: 0.0,
      standardDeduction: 12000.0,
    },
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
      (Pervasives.max_float, 8.82),
    ],
    cityRateSchedule: [(12000.0, 3.078), (25000.0, 3.762), (50000.0, 3.819), (Pervasives.max_float, 3.876)],
    dependentPersonalExemption: 1000.0,
    standardDeduction: 8000.0,
  },
};

let getTaxRates = (year: int) : taxRates =>
  switch (year) {
  | 2014 => getTaxRatesForYear2014Single()
  | 2015 => getTaxRatesForYear2015Single()
  | 2016 => getTaxRatesForYear2016Single()
  | 2017 => getTaxRatesForYear2017Single()
  | 2018 => getTaxRatesForYear2018Single()
  | _ => getTaxRatesForYear2018Single()
  };
