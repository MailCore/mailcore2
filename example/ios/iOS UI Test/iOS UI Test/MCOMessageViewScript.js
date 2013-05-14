
var imageElements = function() {
	var imageNodes = document.getElementsByTagName('img');
	return [].slice.call(imageNodes);
}

var findCIDImageURL = function() {
	var images = imageElements();
	
	var imgLinks = [];
	for (var i = 0; i < images.length; i++) {
		var url = images[i].getAttribute('src');
		if (url.indexOf('cid:') == 0 || url.indexOf('x-mailcore-image:') == 0)
			imgLinks.push(url);
	}
	return JSON.stringify(imgLinks);
}

var replaceImageSrc = function(jsonString) {
	var parsedJson = JSON.parse(jsonString);
	var images = imageElements();
	
	for (var i = 0; i < images.length; i++) {
		var url = images[i].getAttribute('src');
		if (url.indexOf(parsedJson.URLKey) == 0) {
			images[i].setAttribute('src', parsedJson.LocalPathKey);
			break;
		}
	}
}
