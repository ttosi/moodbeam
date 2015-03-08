$(function() {
	$(document).on('click', 'button', function() {
		var cmd = $(this).attr('data-command');
		console.log(cmd);
		
		$.get(serviceUrl + cmd, function(result) {
			console.log(result);
		});
	})
});