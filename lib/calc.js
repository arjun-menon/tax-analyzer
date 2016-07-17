#!/usr/bin/env node

nyc_taxes = require('./nyc-taxes.js')

/*----------------------------------------------------------*
 *  Node.js Command Line Interface (CLI) to `nyc_taxes.js`  *
 *----------------------------------------------------------*/

function w(kind, text, amt) {
    if ( kind == 'a' )
        console.log( text + ' = ' + amt + '\n' )
    else if ( kind == 'l_start' )
        console.log( text + ':' )
    else if ( kind == 'b' )
        console.log( '   ' + text )
    else if ( kind == 'l_end' )
        console.log( text + ' = ' + amt + '\n' )
    else if ( kind == 't' )
        console.log( text )
}

function node_console_main() {
    console_help_message = "\
usage: node calc_nyc_taxes.js [-h] income [-d DEDUCTIONS] [-e EXEMPTIONS]\n\
\n\
Calculates total U.S. taxes owed by a single (unmarried) resident of NYC.\n\
\n\
positional arguments:\n\
  income                Annual Gross Income\n\
\n\
optional arguments:\n\
  -h, --help            show this help message and exit\n\
  -d DEDUCTIONS, --deductions DEDUCTIONS\n\
                        Itemized deduction other than the standard deduction,\n\
                        personal exemptions, and state/local/foreign tax\n\
                        deductions.\n\
  -e EXEMPTIONS, --exemptions EXEMPTIONS\n\
                        Number of personal exemptions. The default is 1."

    if ( process.argv.length < 3 || process.argv[2] == "-h" ) {
        console.log(console_help_message)
    }
    else {
        income = parseInt( process.argv[2] )

        deductions = 0
        exemptions = 1

        for(i = 3; i < (process.argv.length - 1); i += 2) {
            if ( process.argv[i] == '-d' )
                deductions = parseInt( process.argv[i+1] )
            if ( process.argv[i] == '-e' )
                exemptions = parseInt( process.argv[i+1] )
        }

        nyc_taxes.calc_taxes(income, deductions, exemptions, w)
    }
}

if (require.main === module) {
    node_console_main()
}
