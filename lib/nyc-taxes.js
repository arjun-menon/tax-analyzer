/*-------------------------------------------------------------------*
 *  Calculate total U.S. taxes owed by an unmarried resident of NYC  *
 *-------------------------------------------------------------------*/

/*******************.
| Utility Functions |
\*******************/

function p(val, percentage) {
    return (val * percentage) / 100.0;
}

function numberWithCommas(x) { // from http://stackoverflow.com/a/2901298
    return x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ',');
}

function ns(number) { // stringify a number represeting money:
    return '$' + numberWithCommas(number.toFixed(2));
}

/*********************.
| Slab Tax Calculator |
\*********************/

function calc_slab_tax(income, slab_schedule, w) {
    result = 0.0;

    prev_limit = 0;
    for (i in slab_schedule) {
        limit = slab_schedule[i][0];
        rate = slab_schedule[i][1];

        if (income <= limit)
            slab_amt = income - prev_limit;
        else // income > limit
            slab_amt = limit - prev_limit;

        tax_amt = p(slab_amt, rate);

        slab_limit = income > limit ? ns(limit) : ns(income);
        w('b', 'Slab from ' + ns(prev_limit) + ' to ' + slab_limit +
            ' = ' + ns(tax_amt) + ' (at ' + rate + '% on ' + ns(slab_amt) + ')');

        result += tax_amt;

        if (income <= limit)
            break;

        prev_limit = limit;
    }

    return result;
}

/********************.
| The Tax Calculator |
\********************/

function calc_taxes(taxRates, income, itemized_deductions, exemptions, w) {
    cat = 'single_2014';
    // default itemized_deductions = 0
    // default exemptions = 1

    w('a', 'Adjusted Gross Income', ns(income));

    // ------------- New York State & City -------------

    w('l_start', 'New York State Tax Deductions');

    nys_deduction = itemized_deductions;

    if (nys_deduction <= taxRates.new_york_state.standard_deduction[cat]) {
        w('b', 'New York State Standard Deduction = ' +
            ns(taxRates.new_york_state.standard_deduction[cat]));

        nys_deduction = taxRates.new_york_state.standard_deduction[cat];
    }
    else
        w('b', 'Itemized Deductions = ' + ns(itemized_deductions));

    // NYS Personal Exemptions for Dependents
    no_of_dependents = Math.max(exemptions - 1, 0);

    nys_personal_exemptions = no_of_dependents * taxRates.new_york_state.dependent_personal_exemption[cat];

    if (no_of_dependents > 0)
        w('b', 'Personal exemptions for dependents = ' +
            ns(nys_personal_exemptions) + '  (' + no_of_dependents +
            ' x ' + ns(taxRates.new_york_state.dependent_personal_exemption[cat]) + ')');

    nys_deduction += nys_personal_exemptions;

    w('l_end', 'New York State Tax Deductions', ns(nys_deduction));

    nys_taxable_income = Math.max(income - nys_deduction, 0);

    w('a', 'New York State Taxable Income', ns(nys_taxable_income));

    w('l_start', 'New York State Income Tax');

    nys_income_tax = calc_slab_tax(nys_taxable_income,
        taxRates.new_york_state.income_tax[cat], w);

    w('l_end', 'New York State Income Tax', ns(nys_income_tax));

    w('l_start', 'New York City Income Tax');

    nyc_income_tax = calc_slab_tax(nys_taxable_income,
        taxRates.new_york_city.income_tax[cat], w);

    w('l_end', 'New York City Income Tax', ns(nyc_income_tax));

    total_state_and_local_income_tax = nys_income_tax + nyc_income_tax;

    w('a', 'Total New York State & City Taxes', ns(total_state_and_local_income_tax));

    // ------------- Federal -------------

    w('l_start', 'Federal Tax Deductions');

    federal_deduction = itemized_deductions + total_state_and_local_income_tax;

    if (federal_deduction < taxRates.federal.standard_deduction[cat]) {
        federal_deduction = taxRates.federal.standard_deduction[cat];
        w('b', 'Federal Standard Deduction = ' + ns(federal_deduction));
    }
    else {
        w('b', 'State and Local Taxes Deduction = ' + ns(total_state_and_local_income_tax));
        w('b', 'Additional Itemized Deductions = ' + ns(itemized_deductions));
    }

    federal_personal_exemptions = taxRates.federal.personal_exemption[cat] * exemptions;

    w('b', 'Personal Exemptions = ' + ns(federal_personal_exemptions) +
        '  (' + exemptions + ' x ' + ns(taxRates.federal.personal_exemption[cat]) + ')');

    federal_deduction += federal_personal_exemptions;

    w('l_end', 'Federal Tax Deductions', ns(federal_deduction));

    federal_taxable_income = Math.max(income - federal_deduction, 0);

    w('a', 'Federal Taxable Income', ns(federal_taxable_income));

    w('l_start', 'Federal Income Tax');

    federal_income_tax = calc_slab_tax(federal_taxable_income, taxRates.federal.income_tax[cat], w);

    w('l_end', 'Federal Income Tax', ns(federal_income_tax));

    w('l_start', 'Federal Insurance Contributions Act (FICA) Tax');

    social_security_taxable_income = Math.min(income, taxRates.federal.social_security_wage_base[cat]);
    social_security_tax = p(social_security_taxable_income, taxRates.federal.social_security_tax_rate[cat]);

    w('b', "Social Security's Old-Age, Survivors, and Disability Insurance (OASDI) Tax: " +
        ns(social_security_tax) + ' (at ' + taxRates.federal.social_security_tax_rate[cat] + '% flat on ' + ns(income) + ')');

    medicare_tax = p(income, taxRates.federal.medicare_tax_rate[cat]);

    w('b', "Medicare's Hospital Insurance (HI) Tax: " + ns(medicare_tax) +
        ' (at ' + taxRates.federal.medicare_tax_rate[cat] + '% flat on ' + ns(income) + ')');

    fica_tax = social_security_tax + medicare_tax;

    w('l_end', 'Federal Insurance Contributions Act (FICA) Tax', ns(fica_tax));

    total_federal_tax = federal_income_tax + fica_tax;

    w('a', 'Total Federal Taxes', ns(total_federal_tax));

    // ----------------------- Total -----------------------

    total_tax = total_federal_tax + total_state_and_local_income_tax;

    w('a', 'Total Federal, State & Local Taxes', ns(total_tax));

    w('x', '<hr/>');

    income_after_tax = income - total_tax;

    monthly_income_after_tax = income_after_tax / 12.0;

    w('a', 'Income after Taxation', ns(income_after_tax));

    w('a', 'Effective Tax Rate', (total_tax * 100.0 / income).toFixed(2) + '%');

    w('a', 'Monthly Income', ns(monthly_income_after_tax));

    if (itemized_deductions > 0) {
        w('x', '<hr/>');

        income_after_taxes_and_deductions = income_after_tax - itemized_deductions;

        monthly_income_after_taxes_and_deductions = income_after_taxes_and_deductions / 12.0;

        w('a', 'Income after Taxes & Deductible Spending', ns(income_after_taxes_and_deductions));

        w('a', 'Monthly Income after Taxes & Deductible Spending', ns(monthly_income_after_taxes_and_deductions));
    }

    return total_tax;
}

if (!(typeof exports == 'undefined'))
    exports.calc_taxes = calc_taxes;
