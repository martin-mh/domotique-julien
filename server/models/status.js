var mongoose = require('mongoose');

var statusSchema = new mongoose.Schema(
{
  latency: Number, // in milliseconds
  connected: Boolean, // if the card is connected,
  heater: // store heaters status
  {
    kitchen: Boolean,
    firstLivingRoom: Boolean,
    office: Boolean,
    secondLivingRoom: Boolean,
    firstBedroom: Boolean,
    secondBedroom: Boolean,
    waterHeater: Boolean,
  }
});

statusSchema.statics.getEntry = function getEntry(callback)
{
  var newEntry = new this;

  this.findOne(function(err, document)
  {
    if(err) 
    {
      callback(err);
      return;
    }

    if(!document)
    {
      callback(null, newEntry);
      return;
    }

    callback(null, document);
  });
}

module.exports = mongoose.model('Status', statusSchema);