const can = document.getElementById('c');
const ctx = can.getContext('2d');

const POINT_SIZE = 7;

var p = {
    x: 50,
    y: 50
}

function lerp(a, b, t) {
    return a + t * (b - a);
}

function lerpPoint(p0, p1, t) {
    return {
        x: lerp(p0.x, p1.x, t),
        y: lerp(p0.y, p1.y, t)
    }
}

function bezier3(p0, p1, p2, t) {
    const i0 = lerpPoint(p0, p1, t),
          i1 = lerpPoint(p1, p2, t);

    return lerpPoint(i0, i1, t);
}

var b = {
    p0: {
        x: 50, y: 0
    },
    p1: {
        x: 100, y: 100
    },
    p2: {
        x: 50, y: 200
    }
}

function RenderPoint(p, s) {
    ctx.beginPath();
    ctx.arc(p.x, p.y, s, 0, Math.PI * 2, false);
    ctx.fill();
}

function renderCurve(c, s) {
    //render curve part first
    const steps = 30, iStep = 1 / steps;

    ctx.beginPath();
    ctx.moveTo(c.p0.x, c.p0.y);
    for (var t = 0; t < 1; t += iStep) {
        var p = bezier3(c.p0, c.p1, c.p2, t);
        ctx.lineTo(
            p.x,
            p.y
        );
    }

    ctx.stroke();
    //TODO: draw points

    RenderPoint(c.p0, s);
    RenderPoint(c.p1, s);
    RenderPoint(c.p2, s);
}

can.style = 'position: absolute; top: 0px; left: 0px;';

function render() {
    can.width = document.documentElement.clientWidth;
    can.height = document.documentElement.clientHeight;
    ctx.fillStyle = '#333';
    ctx.fillRect(0,0,can.width,can.height);
    ctx.translate(0.5, 0.5);

    //draw main point
    ctx.fillStyle = '#fa0';
    RenderPoint(p, POINT_SIZE);

    //draw curve
    ctx.strokeStyle = '#fff';
    ctx.fillStyle = '#f00';
    renderCurve(b, POINT_SIZE);
}

render();

var targetPoint = null;

var mx = 0, my = 0;

window.addEventListener('mousedown', function(e) {
    var np = 3;
    const rect = can.getBoundingClientRect();
    mx = e.pageX - rect.left;
    my = e.pageY - rect.top;

    for (var i =0 ; i < np; i++) {
        const p = b['p'+i];
        if (Math.sqrt(((mx - p.x)**2) + ((my - p.y)**2)) <= POINT_SIZE) {
            targetPoint = p;
            return;
        }
    }
});

window.addEventListener('mousemove', function(e) {
    const rect = can.getBoundingClientRect();
    mx = e.pageX - rect.left;
    my = e.pageY - rect.top;

    if (targetPoint != null) {
        targetPoint.x = mx;
        targetPoint.y = my;

        render();
    }
})

window.addEventListener('mouseup', function(e) {
    targetPoint = null;
})