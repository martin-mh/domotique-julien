function set(name, status)
{
  HTTP.get('http://192.168.0.50:3006/setHeaterStatus/' + name + '/' + status, function(e, r) { if(e) console.log(e); if(r) console.log(r); });
}

if (Meteor.isClient) {

  Template.content.events({
    'click #cuisine_on': function () {
      set('kitchenHeater', 'on');
    },
    'click #cuisine_off': function()
    {
      set('kitchenHeater', 'off');
    },
    'click #sallon_on': function()
    {
      set('firstLivingRoomHeater', 'on');
    },
    'click #sallon_off': function()
    {
      set('firstLivingRoomHeater', 'off');
    },
    'click #bureau_on': function()
    {
      set('officeHeater', 'on');
    },
    'click #bureau_off': function()
    {
      set('officeHeater', 'off');
    },
    'click #sallon2_on': function()
    {
      set('secondLivingRoomHeater', 'on');
    },
    'click #sallon2_off': function()
    {
      set('secondLivingRoomHeater', 'off');
    },
    'click #chambre_on': function()
    {
      set('firstBedroomHeater', 'on');
    },
    'click #chambre_off': function()
    {
      set('firstBedroomHeater', 'off');
    },
    'click #chambre2_on': function()
    {
      set('secondBedroomHeater', 'on');
    },
    'click #chambre2_off': function()
    {
      set('secondBedroomHeater', 'off');
    },
    'click #chauffeeau_on': function()
    {
      set('waterHeater', 'on');
    },
    'click #chauffeeau_off': function()
    {
      set('waterHeater', 'off');
    }
  });
}

if (Meteor.isServer) {
  Meteor.startup(function () {
    // code to run on server at startup
  });
}
