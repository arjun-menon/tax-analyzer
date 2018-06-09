
var taxRates = {
    /***************.
    | Federal Taxes |
    \***************/
    'federal' : {
        'income_tax': {
            'single_2014': [
                //  (Limit, Rate)
                [9075       , 10],
                [36900      , 15],
                [89350      , 25],
                [186350     , 28],
                [405100     , 33],
                [406750     , 35],
                [Number.MAX_VALUE , 39.6]
            ]
        },
        'medicare_tax_rate': {
            'single_2014': 1.45
        },
        'social_security_tax_rate': {
            'single_2014': 6.2
        },
        'social_security_wage_base': {
            'single_2014': 117000
        },
        'personal_exemption': {
            'single_2014': 3950
        },
        'standard_deduction': {
            'single_2014': 6200
        }
    },

    /*************.
    | State Taxes |
    \*************/
    'new_york_state': {
        'income_tax': {
            'single_2014': [
                //  (Limit, Rate)
                [8200       , 4.00],
                [11300      , 4.50],
                [13350      , 5.25],
                [20550      , 5.90],
                [77150      , 6.45],
                [205850     , 6.65],
                [1029250    , 6.85],
                [Number.MAX_VALUE , 8.82]
            ]
        },
        'dependent_personal_exemption': {
            'single_2014': 1000
        },
        'standard_deduction': {
            'single_2014': 7800
        },
    },

    /************.
    | City Taxes |
    \************/
    'new_york_city': {
        'income_tax': {
            'single_2014': [
                //  (Limit, Rate)
                [12000      , 2.907],
                [25000      , 3.534],
                [50000      , 3.591],
                [500000     , 3.648],
                [Number.MAX_VALUE , 3.876]
            ]
        }
    }
};

if (!(typeof exports == 'undefined'))
    exports.taxRates = taxRates;
