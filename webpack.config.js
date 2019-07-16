const path = require('path');
const outputDir = path.join(__dirname);

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './src/WebInterface.bs.js',
  mode: isProd ? 'production' : 'development',
  output: {
    path: outputDir,
    filename: 'index.js',
  },
  plugins: [
  ],
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: process.env.PORT || 8000,
    historyApiFallback: true
  }
};
