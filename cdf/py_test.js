const { interpolate, range, min, max, random } = require('./functions');
const Cdf = require('./cdf');
const fs = require('fs');
const { CdfCombination } = require('./cdf-combination');

'use strict';


const readCdfs = () => {
    let rawdata = fs.readFileSync("/dev/stdin", "utf-8");
    let cdf_data = JSON.parse(rawdata);  

    cdfs = [];

    for (let cdf of cdf_data) {
        cdfs.push(new Cdf.Cdf(cdf.xs, cdf.ys));
    }

    return cdfs;
}

const main = () => {
    if (process.argv.length > 1) {
        mode = process.argv[2];
        
        if (mode == 'rvs') {
            let N = parseInt(process.argv[3]);

            let cdfs = readCdfs();
            
            RVS = []
            for (let cdf of cdfs){
                rvs = cdf.sample(N);
                RVS.push(rvs);
            }
            console.log(JSON.stringify(RVS));
        }
        else if(mode == 'combine_sample') {
            let cdfs = readCdfs();
            let N = parseInt(process.argv[3]);
            let combineSampleCount = 1000;

            let combined = new CdfCombination(cdfs).combine(combineSampleCount);
            
            let rvs = combined.sample(N);
            console.log(JSON.stringify(rvs));
        }
        else if(mode == 'combine_raw_cdf') {
            let cdfs = readCdfs();
            let N = parseInt(process.argv[3]);
            let combineSampleCount = 1000;

            let combined = new CdfCombination(cdfs).combine(combineSampleCount);
            let output = {
                'xs': combined.xs,
                'ys': combined.ys
            }
            console.log(JSON.stringify(output));
        }
        else if(mode == 'new_range') {
            let cdf = readCdfs()[0];
            let rangeN = parseInt(process.argv[3]);
            resultCdf = cdf.convertToNewLength(rangeN);

            let output = {
                'xs': resultCdf.xs,
                'ys': resultCdf.ys
            }
            console.log(JSON.stringify(output));
        }

    }
}

main();




