const path = require('path');
const outputDir = path.join(__dirname);

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './src/web-interface.js',
  mode: isProd ? 'production' : 'development',
  output: {
    path: outputDir,
    publicPath: outputDir,
    filename: 'index.js',
  },
};
