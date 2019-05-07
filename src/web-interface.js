import * as WebInterface from './WebInterface.bs.js';

const default_income = '100,000';
const first_available_tax_year = 2014;
const last_available_tax_year = 2019;

function getParameterByName(name) {
    // from: http://stackoverflow.com/a/901144/908430
    name = name.replace(/[\[]/, '\\[').replace(/[\]]/, '\\]');
    const regex = new RegExp('[\\?&]' + name + '=([^&#]*)'),
        results = regex.exec(location.search);
    return results == null ? '' : decodeURIComponent(results[1].replace(/\+/g, ' '));
}

function renderReport(input) {
    WebInterface.renderReport(parseInt(input.taxYear), input.income, input.deductions, input.exemptions);
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

function createInputObject(incomeParam, deductionsParam, exemptionsParam, taxYear) {
    const input = {
        income: parseFloat(removeCommas(incomeParam)),
        deductions: parseFloat(removeCommas(deductionsParam)),
        exemptions: parseFloat(removeCommas(exemptionsParam)),
        taxYear: taxYear
    };

    if (isValidNumber(incomeParam) && input.income > 0 &&
        isValidNumber(deductionsParam) && input.deductions >= 0 &&
        isValidNumber(exemptionsParam) && input.exemptions >= 0)
        return input;
    else
        throw constructErrorMessage(input, incomeParam, deductionsParam, exemptionsParam);
}

function setFormValues(incomeVal, deductionsVal, exemptionsVal, taxYearVal) {
    document.getElementById('agi').value = incomeVal;
    document.getElementById('itd').value = deductionsVal;
    document.getElementById('npe').value = exemptionsVal;
    document.getElementById('taxYearSelector').value = taxYearVal;
}

function getParams() {
    let incomeParam = getParameterByName('income');
    let deductionsParam = getParameterByName('deductions');
    let exemptionsParam = getParameterByName('exemptions');
    let taxYearParam = getParameterByName('taxYear');

    // if there are no parameters, use default income
    if (incomeParam === '' && deductionsParam === '' && exemptionsParam === '')
        incomeParam = default_income;

    if (deductionsParam == '')
        deductionsParam = '0';

    if (exemptionsParam == '')
        exemptionsParam = '1';

    if (!(parseInt(taxYearParam) >= first_available_tax_year &&
          parseInt(taxYearParam) <= last_available_tax_year))
        taxYearParam = `${last_available_tax_year}`;

    setFormValues(incomeParam, deductionsParam, exemptionsParam, taxYearParam);

    return createInputObject(incomeParam, deductionsParam, exemptionsParam, taxYearParam);
}

function getFormParams() {
    return {
        income: document.getElementById('agi').value,
        deductions: document.getElementById('itd').value,
        exemptions: document.getElementById('npe').value,
        taxYear: document.getElementById('taxYearSelector').value
    };
}

function createInputObjectFromParams(formParams) {
    return createInputObject(formParams.income, formParams.deductions, formParams.exemptions, formParams.taxYear);
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

        renderReport(input);
    }
    catch (err_output) {
        history.pushState(formParams, '', '?' + encodeQueryData(formParams));
        console.error(err_output);
        setResults(err_output);
    }
}

function main() {
    try {
        renderReport(getParams());
    }
    catch (err_output) {
        console.error(err_output);
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

                renderReport(input);
            }
            catch (err_output) {
                console.error(err_output);
                setResults(err_output);
            }
        };
    }

    document.getElementById('taxYearSelector').onchange = dynamicSubmit;
}

if (typeof window !== 'undefined') {
    main();
}