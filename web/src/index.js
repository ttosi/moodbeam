var serviceUrl = 'http://24.21.135.194:1503';

$(function() {
	$(document).on('click', 'button', function() {
		var cmd = $(this).attr('data-command');
		console.log(cmd);
		
		$.get(serviceUrl + cmd, function(result) {
			console.log(result);
		});
	})
});