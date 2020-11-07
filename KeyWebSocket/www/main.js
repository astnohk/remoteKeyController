"use strict";

//
// CONFIG
//

const WebSocket_port = 50001;

//
//
//


// Polygon for UI
let backwardPoly = [
    // left triangle
    [
	    0.5, 0.5,
	    0.5, 0.1,
	    0.1, 0.5,
	    0.5, 0.9,
	    0.5, 0.5,
	    // next triangle
	    0.5, 0.5,
	    0.9, 0.9,
	    0.9, 0.1,
	    0.5, 0.5,
    ],
];
let playStopPoly = [
    [
	    // left bar
	    0.2, 0.1,
	    0.2, 0.9,
	    0.3, 0.9,
	    0.3, 0.1,
    ],
    [
	    // next bar
	    0.4, 0.1,
	    0.4, 0.9,
	    0.5, 0.9,
	    0.5, 0.1,
    ],
    [
	    // right triangle
	    0.6, 0.1,
	    0.6, 0.9,
	    0.9, 0.5,
    ],
];
let forwardPoly = [
    [
	    // left triangle
	    0.1, 0.1,
	    0.1, 0.9,
	    0.5, 0.5,
    ],
    [
	    // next triangle
	    0.5, 0.1,
	    0.5, 0.9,
	    0.9, 0.5,
    ],
];


let pattern_URL = /http:\/\/([^:\/]+)(:[^\/]+|\/)/;
let URL = window.location.href;
let WebSocketURL;
{
	let m = URL.match(pattern_URL);
	if (m) {
		WebSocketURL = "ws://" + m[1] + ":" + WebSocket_port + "/";
	} else {
		WebSocketURL = "ws://127.0.0.1:" + WebSocket_port + "/";
	}
}
let websock;

window.onload = init;


function init()
{
	// Connect WebSocket
	websock = new WebSocket(WebSocketURL);
	websock.onmessage = (e) => { console.log(e.data); };

	// Add Event
	const backward_button = document.getElementById("backward_button");
	backward_button.addEventListener(
	    "click",
	    (e) => {
		    websock.send("backward");
	    });
	const playStop_button = document.getElementById("playStop_button");
	playStop_button.addEventListener(
	    "click",
	    (e) => {
		    websock.send("playStop");
	    });
	const forward_button = document.getElementById("forward_button");
	forward_button.addEventListener(
	    "click",
	    (e) => {
		    websock.send("forward");
	    });

	// Start drawing the UI
	requestAnimationFrame(render);
}


function render()
{
	let style;

	const backward_canvas = document.getElementById("backward_button_canvas");
	style = backward_canvas.getBoundingClientRect()
	backward_canvas.width = style.width;
	backward_canvas.height = style.height;
	drawPoly(backward_canvas, backwardPoly);

	const playStop_canvas = document.getElementById("playStop_button_canvas");
	style = playStop_canvas.getBoundingClientRect()
	playStop_canvas.width = style.width;
	playStop_canvas.height = style.height;
	drawPoly(playStop_canvas, playStopPoly);

	const forward_canvas = document.getElementById("forward_button_canvas");
	style = forward_canvas.getBoundingClientRect()
	forward_canvas.width = style.width;
	forward_canvas.height = style.height;
	drawPoly(forward_canvas, forwardPoly);

	// loop
	requestAnimationFrame(render);
}

function drawPoly(canvas, polygon)
{
	const w = canvas.width;
	const h = canvas.height;
	let p = new Path2D();
	for (let i = 0; i < polygon.length; ++i) {
		const shape = polygon[i];
		p.moveTo(shape[0] * w, shape[1] * h);
		for (let k = 2; k < shape.length; k += 2) {
			p.lineTo(shape[k] * w, shape[k + 1] * h);
		}
	}
	p.closePath();

	const ctx = canvas.getContext("2d");
	ctx.fillStyle = "rgb(255,255,255)";
	ctx.fill(p);
}

