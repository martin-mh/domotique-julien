var mongoose = require('mongoose');

var wattsHistorySchema = new mongoose.Schema(
{
  watts: Number,
  from: Date,
  to:
  {
    type: Date,
    default: Date.now,
  },
  type: String,
});

wattsHistorySchema.statics.createEntry = function createEntry(watts, type)
{
  var entry = new this;
  entry.watts = watts;
  entry.type = type;
  entry.from = new Date(entry.to.getTime() - 30000);

  entry.save();
}

module.exports = mongoose.model('WattsHistory', wattsHistorySchema);