var serviceUrl = '24.21.135.194:1503';

$(function() {
	$.get(serviceUrl + '/setcolor/0/255/0', function(result) {
		console.log(result);
	});
})();