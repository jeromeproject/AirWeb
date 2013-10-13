function get_param_from_query_string(name) {
    name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
        results = regex.exec(location.search);
    return results == null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
}

function get_user_list(cb, QueryString /*usless*/, UserList)
{
  var url_string = 'User.exe?Action=list' + '&time=' + $.now();
  //alert(url_string);
  $('#'+UserList).val("");
  $.ajax({
	url: url_string,
	dataType: "text",
    contentType: 'text/plain;charset=utf-8',
	error: function(xhr) {
	  alert(url_string + ' request failed');
	  if(cb) cb(false, null);
	},
	success: function(response) {
		//response = escape(response);
		//alert(response);
		$('#'+UserList).val(response);
		if(cb) cb(true, response);
	}
  });
}

function ajax_get_login_info(cb, LoginUser, Group)
{
  var url_string_group = 'User.exe?Action=info' + '&time=' + $.now();
  $.ajax({
	url: url_string_group,
	contentType: 'text/plain;charset=utf-8',
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
			if(!f[1])
				continue;
			//alert(f[0] + " = " + f[1]);
			f[1] = f[1].trim();
			if(LoginUser && f[0] == "LoginUser")
			{
				$('#'+LoginUser).val(f[1]);
			}
			else if(Group && f[0] == "Group")
			{
				$('#'+Group).val(f[1]);
			}
		}
		if(cb) cb(true, null);
	}
  });
}

function ajax_get_user_profile(cb, who, PlainPassword, PlainPasswordConfirm, Nickname, Comment, FriendList, BlackList)
{
  var url_string = 'User.exe?Action=get&username=' + who + '&time=' + $.now();
  $.ajax({
	url: url_string,
	contentType: 'text/plain;charset=utf-8',
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
			f[1] = f[1].trim();
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
			else if(f[0] == "StatusComment")
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
	var cNickname = encodeURIComponent($('#'+Nickname).val());
	var cComment = encodeURIComponent($('#'+Comment).val());
	var cFriendList = encodeURIComponent($('#'+FriendList).val());
	var cBlackList = encodeURIComponent($('#'+BlackList).val());
	// var cNickname = $('#'+Nickname).val();
	// var cComment = $('#'+Comment).val();
	// var cFriendList = $('#'+FriendList).val();
	// var cBlackList = $('#'+BlackList).val();
	var url_string = 'User.exe?Action=update&username=' + who
		+ "&PlainPassword=" + $('#'+PlainPassword).val()
		+ "&Nickname=" + cNickname
		+ "&StatusComment=" + cComment
		+ "&FriendList=" + cFriendList
		+ "&BlackList=" + cBlackList
		+ '&time=' + $.now()
		;
	//url_string = encodeURIComponent(url_string);
	//alert(url_string);
	$.ajax({
		url: url_string,
		contentType: 'text/plain;charset=utf-8',
		error: function(xhr) {
			if(cb) cb(false, url_string + ' request failed');
		},
		success: function(response) {
			if(response.toLowerCase().indexOf("ok") == -1)
			{
				//alert(response);
				if(cb) cb(false, response);
			}
			else
				if(cb) cb(true, null);
		}
	});
}
 
 function ajax_delete_user_profile(cb, who)
 {
	//who = encodeURIComponent(who);
	var url_string = 'User.exe?Action=del&username=' + who + '&time=' + $.now();
	//alert(url_string);
	$.ajax({
		url: url_string,
		contentType: 'text/plain;charset=utf-8',
		error: function(xhr) {
			if(cb) cb(false, url_string + ' request failed');
		},
		success: function(response) {
			//alert(response);	
			if(response.toLowerCase().indexOf("ok") == -1)
			{
				if(cb) cb(false, response);
			}
			else
				if(cb) cb(true, null);
		}
	});
}
 
function ajax_create_user_profile(cb, Username, PlainPassword, PlainPasswordConfirm, Nickname, Comment, FriendList, BlackList)
{
	var url_string = 'User.exe?Action=add&username=' + $('#'+Username).val()
		+ "&PlainPassword=" + $('#'+PlainPassword).val()
		+ '&time=' + $.now()
		;
	//url_string = encodeURIComponent(url_string);
	//alert(url_string);
	$.ajax({
		url: url_string,
		contentType: 'text/plain;charset=utf-8',
		error: function(xhr) {
			if(cb) cb(false, url_string + ' request failed');
		},
		success: function(response) {
			//alert(response);
			if(response.toLowerCase().indexOf("ok") == -1)
			{
				//alert(response);
				if(cb) cb(false, response);
			}
			else
				if(cb) cb(true, null);
		}
	});
}