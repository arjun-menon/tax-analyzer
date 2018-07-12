
import * as TaxRates from './TaxRates.bs.js';
import * as TaxCalc from './TaxCalc.bs.js';

const default_income = '50,000';

function getParameterByName(name) {
    // from: http://stackoverflow.com/a/901144/908430
    name = name.replace(/[\[]/, '\\[').replace(/[\]]/, '\\]');
    const regex = new RegExp('[\\?&]' + name + '=([^&#]*)'),
        results = regex.exec(location.search);
    return results == null ? '' : decodeURIComponent(results[1].replace(/\+/g, ' '));
}

function createHtmlFragment(htmlStr) {
    // from: http://stackoverflow.com/a/814649/908430
    // also, see: http://ejohn.org/blog/dom-documentfragments/
    const frag = document.createDocumentFragment(),
        temp = document.createElement('div');
    temp.innerHTML = htmlStr;
    while (temp.firstChild) {
        frag.appendChild(temp.firstChild);
    }
    return frag;
}

function setResults(htmlStr) {
    const results = document.getElementById('results');
    results.innerHTML = '';

    const fragment = createHtmlFragment(htmlStr);
    results.appendChild(fragment);

    if (htmlStr != '')
        results.style.visibility = 'visible';
    else
        results.style.visibility = 'hidden';
}

function calculateAndDisplayTaxes(income, deductions, exemptions) {
    let resultsHtml = '';

    function w(kind, text, amt) {
        if (kind == 'a')
            resultsHtml += '<p>' + text + ': ' + amt + '</p>';
        else if (kind == 'l_start')
            resultsHtml += '<div class="seclist"><p><strong>' + text + '</strong><ul>';
        else if (kind == 'b')
            resultsHtml += '<li>' + text + '</li>';
        else if (kind == 'l_end')
            resultsHtml += '</ul><em>' + text + ':</em> ' + amt + '</p></div>';
        else if (kind == 't')
            resultsHtml += '<p>' + text + '</p>';
        else if (kind == 'x') // html decorators
            resultsHtml += text;
    }

    function do_nothing_w(kind, text, amt) {}

    const my_tax = TaxCalc.calc_taxes(TaxRates.getTaxRates(), income, deductions, exemptions, w),
        my_tax_plus = TaxCalc.calc_taxes(TaxRates.getTaxRates(), income + 1.0, deductions, exemptions, do_nothing_w);

    const marginal_tax_rate = (my_tax_plus - my_tax) * 100.0;
    w('a', 'Marginal Tax Rate', marginal_tax_rate.toFixed(2) + '%');

    return resultsHtml;
}

function calcAndDisplay(input) {
    setResults(calculateAndDisplayTaxes(input.income, input.deductions, input.exemptions));
}

function constructErrorMessage(input, incomeParam, deductionsParam, exemptionsParam) {
    let errorMessage = '';

    if (!incomeParam)
        errorMessage += 'Income must be specified. <br />';
    if (incomeParam && input.income == 0)
        errorMessage += 'Income must be greater than zero. <br />';
    if (incomeParam && (!isValidNumber(incomeParam) || !(input.income >= 0)))
        errorMessage += "Income: '" + incomeParam + "'" + ' is not valid. <br />';
    if (!deductionsParam)
        errorMessage += 'The total dollar amount of tax deductions must be specified. <br />';
    if (deductionsParam && (!isValidNumber(deductionsParam) || !(input.deductions >= 0)))
        errorMessage += "Deductions: '" + deductionsParam + "'" + ' is not valid. <br />';
    if (!exemptionsParam)
        errorMessage += 'The number of personal exemptions must be specified. <br />';
    if (exemptionsParam && (!isValidNumber(exemptionsParam) || !(input.exemptions >= 0)))
        errorMessage += "Exemptions: '" + exemptionsParam + "'" + ' is not valid. <br />';

    return errorMessage;
}

function isValidNumber(text) { // from: http://stackoverflow.com/a/21907572/908430
    return /^[0-9,.]*$/.test(text);
}

function removeCommas(text) {
    return text.replace(/,/g, '');
}

function createInputObject(incomeParam, deductionsParam, exemptionsParam) {
    const input = {
        income: parseFloat(removeCommas(incomeParam)),
        deductions: parseFloat(removeCommas(deductionsParam)),
        exemptions: parseFloat(removeCommas(exemptionsParam))
    };

    if (isValidNumber(incomeParam) && input.income > 0 &&
        isValidNumber(deductionsParam) && input.deductions >= 0 &&
        isValidNumber(exemptionsParam) && input.exemptions >= 0)
        return input;
    else
        throw constructErrorMessage(input, incomeParam, deductionsParam, exemptionsParam);
}

function setFormValues(incomeVal, deductionsVal, exemptionsVal) {
    document.getElementById('agi').value = incomeVal;
    document.getElementById('itd').value = deductionsVal;
    document.getElementById('npe').value = exemptionsVal;
}

function getParams() {
    let incomeParam = getParameterByName('income');
    let deductionsParam = getParameterByName('deductions');
    let exemptionsParam = getParameterByName('exemptions');

    // if there are no parameters, use default income
    if (incomeParam === '' && deductionsParam === '' && exemptionsParam === '')
        incomeParam = default_income;

    if (deductionsParam == '')
        deductionsParam = '0';

    if (exemptionsParam == '')
        exemptionsParam = '1';

    setFormValues(incomeParam, deductionsParam, exemptionsParam);

    return createInputObject(incomeParam, deductionsParam, exemptionsParam);
}

function getFormParams() {
    return {
        income: document.getElementById('agi').value,
        deductions: document.getElementById('itd').value,
        exemptions: document.getElementById('npe').value
    };
}

function createInputObjectFromParams(formParams) {
    return createInputObject(formParams.income, formParams.deductions, formParams.exemptions);
}

function overrideSubmit(onsubmit_function) {
    // from: http://stackoverflow.com/a/1395254
    // also see: http://stackoverflow.com/a/3350260
    function jsSubmit() {
        onsubmit_function();
        return false;
    }
    document.forms['input_form'].onsubmit = jsSubmit;
}

function doesSupportHistoryAPI() {
    // from: http://stackoverflow.com/a/9446306
    return !!(window.history && history.pushState);
}

function encodeQueryData(data) {
    // from: http://stackoverflow.com/a/111545
    const ret = [];
    for (const d in data)
        ret.push(encodeURIComponent(d) + '=' + encodeURIComponent(data[d]));
    return ret.join('&');
}

function dynamicSubmit() {
    const formParams = getFormParams();

    try {
        const input = createInputObjectFromParams(formParams);

        history.pushState(formParams, '', '?' + encodeQueryData(formParams));

        calcAndDisplay(input);
    }
    catch (err_output) {
        history.pushState(formParams, '', '?' + encodeQueryData(formParams));

        setResults(err_output);
    }
}

function main() {
    try {
        calcAndDisplay(getParams());
    }
    catch (err_output) {
        setResults(err_output);
    }

    if (doesSupportHistoryAPI()) {
        overrideSubmit(dynamicSubmit);

        window.onpopstate = function(event) {
            const formParams = event.state;

            if (formParams)
                setFormValues(formParams.income, formParams.deductions, formParams.exemptions);

            try {
                const input = formParams ? createInputObjectFromParams(formParams) : getParams();

                calcAndDisplay(input);
            }
            catch (err_output) {
                setResults(err_output);
            }
        };
    }
}

main();
