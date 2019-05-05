const path = require('path');
// const HtmlWebpackPlugin = require('html-webpack-plugin');
const outputDir = path.join(__dirname);

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './src/web-interface.js',
  mode: isProd ? 'production' : 'development',
  output: {
    path: outputDir,
    // publicPath: outputDir,
    filename: 'index.js',
  },
  plugins: [
  //  new HtmlWebpackPlugin({
  //    template: 'index.html',
  //    inject: false
  //  })
  ],
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: process.env.PORT || 8000,
    historyApiFallback: true
  }
};
