var serviceUrl = 'http://192.168.15.30:1503';

$(function() {
	$(document).on('click', 'button', function() {
		var cmd = $(this).attr('data-command');
		console.log(cmd);
		
		$.get(serviceUrl + cmd, function(result) {
			console.log(result);
		});
	})
});