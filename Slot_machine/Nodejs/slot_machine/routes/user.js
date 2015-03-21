var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/:username', function(req, res, next) {
  res.send('Hello ' + req.params.username + "<br />" +
  	'Now is ' + new Date().toString());
  // res.send('Now is ' + new Date().toString());
});

module.exports = router;
