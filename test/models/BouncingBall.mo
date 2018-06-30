model Ball
    Real x;
    Real v;
    parameter Real g = 9.8;
equation
    der(x) = v;
    der(v) = -g;
    when (x < 0) then
        v = -v;
    end when;
end Ball;

// vim: set et ft=modelica fenc=utf-8 ff=unix sts=0 sw=4 ts=4 :

