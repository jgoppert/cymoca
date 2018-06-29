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