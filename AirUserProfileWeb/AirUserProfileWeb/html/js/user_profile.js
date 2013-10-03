function get_param_from_query_string(name) {
    name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
        results = regex.exec(location.search);
    return results == null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
}

function ajax_get_login_info(cb, LoginUser, Group)
{
  var url_string_group = 'User.exe?Action=info';
  $.ajax({
	url: url_string_group,
	error: function(xhr) {
	  alert(url_string_group + ' request failed');
	  if(cb) cb(false, null);
	},
	success: function(response) {
		var ent = response.split("\r\n");
		//alert(response);
		for(var i = 0; i < ent.length; i++)
		{
			var f = ent[i].split("=");
			//alert(f[0] + " = " + f[1]);
			f[1].trim();
			if(LoginUser && f[0] == "LoginUser")
			{
				$('#'+LoginUser).text(f[1]);
			}
			else if(Group && f[0] == "Group")
			{
				$('#'+Group).text(f[1]);
			}
		}
		if(cb) cb(true, null);
	}
  });
}

function ajax_get_user_profile(cb, who, PlainPassword, PlainPasswordConfirm, Nickname, Comment, FriendList, BlackList)
{
  var url_string = 'User.exe?Action=get&username=' + who;
  $.ajax({
	url: url_string,
	error: function(xhr) {
		alert(url_string + ' request failed');
		if(cb)
			cb(false);
	},
	success: function(response) {
		var ent = response.split("\r\n");
		//alert(response);
		if(ent.length < 4)
		{
			//alert(response);
			if(cb)
				cb(false, response);
			return;
		}
		for(var i = 0; i < ent.length; i++)
		{
			var f = ent[i].split("=");
			//alert("info: " + ent[i]);
			if(f.length == 1)
			{
				//alert("error: " + ent[i]);
				continue;
			}
			f[1].trim();
			//alert("info: " + f[1]);
			if(f[0] == "PlainPassword")
			{
				//$('[name="'+PlainPassword+'"]').val(f[1]);
				$('#'+PlainPassword).val(f[1]);
				$('#'+PlainPasswordConfirm).val(f[1]);
			}
			/*
			else if(f[0] == "UserPL")
			{
				$('#'+UserPL).val(f[1]);
			}
			*/
			else if(f[0] == "Nickname")
			{
				$('#'+Nickname).val(f[1]);
			}
			else if(f[0] == "Comment")
			{
				$('#'+Comment).val(f[1]);
			}
			else if(f[0] == "FriendList")
			{
				$('#'+FriendList).val(f[1]);
			}
			else if(f[0] == "BlackList")
			{
				$('#'+BlackList).val(f[1]);
			}
		}
		if(cb)
			cb(true, null);
	}
  });
 }
 
 function ajax_update_user_profile(cb, who, PlainPassword, PlainPasswordConfirm, Nickname, Comment, FriendList, BlackList)
{
  var url_string = 'User.exe?Action=update&username=' + who
						+ "&PlainPassword=" + $('#'+PlainPassword).val()
						+ "&Nickname=" + $('#'+Nickname).val()
						+ "&Comment=" + $('#'+Comment).val()
						+ "&FriendList=" + $('#'+FriendList).val()
						+ "&BlackList=" + $('#'+BlackList).val()
						;
	//alert(url_string);
	$.ajax({
		url: url_string,
		error: function(xhr) {
		  alert(url_string + ' request failed');
		},
		success: function(response) {
			if(response.toLowerCase().indexOf("ok") == -1)
			{
				alert(response);
				cb(false, response);
			}
			cb(true, null);
		}
	});
 }