  var url_string = 'User.exe?Action=get&username=person';
  $.ajax({
	url: url_string,
	error: function(xhr) {
	  alert('Ajax request failed');
	},
	success: function(response) {
	  $('#msg').html(response);
	  $('#msg').fadeIn();
	}
  });
