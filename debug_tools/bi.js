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

function epsilize(v) {
    const e = 0.0001, ie = 1 / e;
    return Math.floor(v * ie) * e;
}

function getRoots(a, b, c) {
    if (epsilize(a) == 0)
        return {
            nRoots: 0
        }

    var root = epsilize(
        (b*b) - (4*a*c)
    ), ida = 1 / (2 * a);

    if (root < 0)
        return {
            nRoots: 0
        }

    root = epsilize(Math.sqrt(root));

    if (root != 0)
        return {
            r0: (-b + root) * ida,
            r1: (-b - root) * ida,
            nRoots: 2
        };
    else
        return {
            r0: (-b + root) * ida,
            nRoots: 1
        };
}

function intersectsCurve(curve, e) {
    var p0 = {
        x: curve.p0.x,
        y: curve.p0.y - e.y
    }, p1 = {
        x: curve.p1.x,
        y: curve.p1.y - e.y
    }, p2 = {
        x: curve.p2.x,
        y: curve.p2.y - e.y
    };

    const a = (p0.y - 2 * p1.y) + p2.y, b = 2 * p1.y - 2 * p0.y, c = p0.y;

    var _roots = getRoots(a, b, c);

    const a2 = (p0.x - 2 * p1.x) + p2.x, b2 = 2 * p1.x - 2 * p0.x, c2 = p0.x;

    var _roots2 = getRoots(a2, b2, c2);

    console.log(_roots);

    if (_roots.nRoots <= 0) return 0; //no roots so no intersection

    //check le roots
    var nRoots = 0;

    //_roots.r0 -= e.x;
    //_roots.r1 -= e.x;

    if (                     _roots.r0 >= 0 && _roots.r0 <= 1 && _roots2.r0 >= 0) nRoots++;
    if (_roots.nRoots > 1 && _roots.r1 >= 0 && _roots.r1 <= 1) nRoots++;
    return nRoots;
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

    //intersection test
    var nIntersections = intersectsCurve(b, p);

    ctx.strokeStyle = '#fff';

    if (nIntersections > 0)
        ctx.strokeStyle = '#0f0';

    //draw curve
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

    //check to see if intersection between main point
    if (Math.sqrt(((mx - p.x)**2) + ((my - p.y)**2)) <= POINT_SIZE)
        targetPoint = p;
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