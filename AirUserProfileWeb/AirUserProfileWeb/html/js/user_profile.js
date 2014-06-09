function get_param_from_query_string(name) {
    name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
        results = regex.exec(location.search);
    return results == null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
}

function get_user_list(cb, QueryString /*usless*/, UserList)
{
  var url_string = 'User.exe?Action=list' + '&QueryString=' + QueryString + '&time=' + $.now();
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

function get_friend_list(cb, QueryString, UserList)
{
	// this function is valid for Doctor group
	var friend_list = "";
	var oUser = ajax_get_user_profile_async(QueryString);
	var ent = oUser.FriendList.split(" ");
	for(var i = 0; i < ent.length; i++)
	{
		if(friend_list == "")
			friend_list += ent[i];
		else
			friend_list += "\n" + ent[i];
	}
	cb("ok", friend_list);
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

function strip_quote(str)
{
	if(!str)
		return "";
	str = str.trim();
	if(str[0] == '"' && str[str.length - 1] == '"')
	{
		//alert(str);
		//str[0] = str[str.length] = '';
		return str.substring(1, str.length-1);
	}
	if(str[0] == '"')
	{
		return str.substring(1, str.length);
	}
	if(str[str.length - 1] == '"')
	{
		//str[0] = str[str.length] = '';
		return str.substring(0, str.length-1);
	}
	return str;
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
		var userpl;
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
			else if(f[0] == "UserPL")
			{
				// $('#'+UserPL).val(f[1]);
				userpl = f[1];
			}
			else if(f[0] == "Nickname")
			{
				$('#'+Nickname).val(f[1]);
			}
			else if(f[0] == "StatusComment")
			{
				$('#'+Comment).val(strip_quote(f[1]));
			}
			else if(f[0] == "FriendList")
			{
				$('#'+FriendList).val(strip_quote(f[1]));
			}
			else if(f[0] == "BlackList")
			{
				$('#'+BlackList).val(strip_quote(f[1]));
			}
		}
		if(cb)
			cb(true, null, userpl);
	}
  });
 }

function ajax_get_user_profile_async(who)
{
	var oUser = new Object;
	oUser.UserName = who;
	oUser.UserPL = -1;
	var url_string = 'User.exe?Action=get&username=' + who + '&time=' + $.now();
	$.ajax({
		url: url_string,
		cache: false,
		async: false,
		contentType: 'text/plain;charset=utf-8',
		error: function(xhr) {
			alert(url_string + ' request failed');
		},
		success: function(response) {
			var ent = response.split("\r\n");
			//alert(response);
			if(ent.length < 4)
			{
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
					oUser.PlainPassword = f[1];
				else if(f[0] == "UserPL")
				{
					oUser.UserPL = f[1];
				}
				else if(f[0] == "Nickname")
				{
					oUser.Nickname = f[1];
				}
				else if(f[0] == "StatusComment")
				{
					oUser.Comment = strip_quote(f[1]);
				}
				else if(f[0] == "FriendList")
				{
					oUser.FriendList = strip_quote(f[1]);
				}
				else if(f[0] == "BlackList")
				{
					oUser.BlackList = strip_quote(f[1]);
				}
			}
		}
	});
  
	return oUser;
 }
 
function ajax_update_user_profile(cb, who, PlainPassword, PlainPasswordConfirm, Nickname, Comment, FriendList, BlackList)
{
	var cNickname = encodeURIComponent($('#'+Nickname).val());
	var cComment = encodeURIComponent('"' + $('#'+Comment).val() + '"');
	var cFriendList = encodeURIComponent('"' + $('#'+FriendList).val() + '"');
	var cBlackList = encodeURIComponent('"' + $('#'+BlackList).val() + '"');
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
			//alert(response);
			if(response.toLowerCase().indexOf("ok") == -1)
			{
				if(cb) cb(false, response);
			}
			else
				if(cb) cb(true, response);
		}
	});
}

function ajax_update_user_friendlist(cb, who, FriendList)
{
	var cFriendList = encodeURIComponent('"' + FriendList + '"');
	var url_string = 'User.exe?Action=update&username=' + who
		+ "&FriendList=" + cFriendList
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
				if(cb) cb(false, response);
			}
			else
				if(cb) cb(true, response);
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
 
function ajax_create_user_profile(cb, UserPL, QueryString, Username, PlainPassword, PlainPasswordConfirm, Nickname, Comment, FriendList, BlackList)
{
	var valUserPL = -1;
	switch(UserPL)
	{
	case "unit":
		valUserPL = 1;
		break;
	default:
		valUserPL = 2;	// doctor
	}
	var url_string = 'User.exe?Action=add&username=' + $('#'+Username).val()
		+ "&UserPL=" + valUserPL
		+ "&QueryString=" + QueryString
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

function ajax_get_friendlist(cb, who)
{
//http://127.0.0.1:8080/User.exe?action=fdlist&QueryString=PersonTest
  var url_string = 'User.exe?Action=fdlist&QueryString=' + who + '&time=' + $.now();
  $.ajax({
	url: url_string,
	contentType: 'text/plain;charset=utf-8',
	error: function(xhr) {
		alert(url_string + ' request failed');
		if(cb)
			cb(false, "");
	},
	success: function(response) {
		//alert(response);
		if(cb)
			cb(true, response);
	}
  });	
}

//GET /User.exe?Action=fdadd&username=_D_test1&Nickname=%22H7878%20abcde%22&QueryString=person1&time=1401285399800 HTTP/1.1
function ajax_create_friend_profile(cb, DeviceID, Nickname, QueryString)
{
	var cNickname = encodeURIComponent('"' + Nickname + '"');
	var url_string = 'User.exe?Action=fdadd&username=' + DeviceID
		+ "&Nickname=" + cNickname
		+ "&QueryString=" + QueryString
		+ '&time=' + $.now()
		;
	//url_string = encodeURIComponent(url_string);
	alert(url_string);
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

//http://127.0.0.1:8080/User.exe?action=fddel&Username=_D_person2
 function ajax_delete_friend_profile(cb, who)
 {
	//who = encodeURIComponent(who);
	var url_string = 'User.exe?Action=fddel&Username=' + who + '&time=' + $.now();
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

 function ajax_update_friend_profile(cb, who, Nickname)
{
//http://127.0.0.1:8080/User.exe?action=fdlist&QueryString=PersonTest
 	var cNickname = encodeURIComponent('\"' + Nickname + '\"');
	var url_string = 'User.exe?Action=update&username=' + who		// should use fdupdate
		+ "&Nickname=" + cNickname
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
				if(cb) cb(false, response);
			}
			else
				if(cb) cb(true, response);
		}
	});
}