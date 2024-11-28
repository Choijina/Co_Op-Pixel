//client.js
const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const PIXEL_SIZE = 5;
const ws = new WebSocket('ws://10.0.2.15:8080');

const colorPicker = document.getElementById('colorPicker');

canvas.addEventListener('click', (event) => {
  const x = Math.floor(event.offsetX / PIXEL_SIZE);
  const y = Math.floor(event.offsetY / PIXEL_SIZE);
 
  const colorHex = colorPicker.value;
  const color = hexToRgb(colorHex);

  const message = `${x} ${y} ${color.r} ${color.g} ${color.b}`;
  ws.send(message);
  drawPixel(x, y, color); // 로컬에서도 그리기
});

ws.onmessage = (message) => {
  const [x, y, r, g, b] = message.data.split(' ').map(Number);
  drawPixel(x, y, { r, g, b });
};

function drawPixel(x, y, color) {
  ctx.fillStyle = `rgb(${color.r}, ${color.g}, ${color.b})`;
  ctx.fillRect(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
}

function hexToRgb(hex){
	const bigint = parseInt(hex.slice(1), 16);
	const r = (bigint >> 16) & 255;
	const g = (bigint >> 8) & 255;
	const b = bigint & 255;
	return {r,g,b};
}