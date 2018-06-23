model Ball
    Real x;
    Real v;
equation
    der(x) = v;
    der(v) = -9.8;
    when (x < 0) then
        v = -v;
    end when;
end Ball;