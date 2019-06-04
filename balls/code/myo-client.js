const Max = require('max-api');
const Myo = require('myo');
const ws = require('ws');

Myo.connect('com.oren.myotest', ws);

Myo.on();