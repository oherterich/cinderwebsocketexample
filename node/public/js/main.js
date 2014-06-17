var ws = new WebSocket("ws://" + window.location.hostname + ":8081"); //Connect to the websocket.

var target = document.getElementById('target');
var targetPos = { "x": 0, "y": 0 };

document.body.addEventListener('mousedown', function(evt) {
	targetPos.x = evt.clientX;
	targetPos.y = evt.clientY;
	target.style.left = targetPos.x;
	target.style.top = targetPos.y;

	var msg = "change-pos," + targetPos.x + "," + targetPos.y;
	ws.send( msg ); //send this data using the socket connection
});