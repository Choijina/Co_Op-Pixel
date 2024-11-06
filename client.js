const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const PIXEL_SIZE = 5;
const ws = new WebSocket('ws://106.101.132.195:8080');

canvas.addEventListener('click', (event) => {
  const x = Math.floor(event.offsetX / PIXEL_SIZE);
  const y = Math.floor(event.offsetY / PIXEL_SIZE);
  const color = { r: 0, g: 0, b: 255 };

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
