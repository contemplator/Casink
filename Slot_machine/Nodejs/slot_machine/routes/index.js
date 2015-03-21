var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  // res.render('index', { title: 'Express' }); // jade
  res.sendfile('./views/index.html'); // html
});

module.exports = router;
