function y = S(x)
% S - the nonlinearity in the BP feedback system
%
% y=S(x) is the vector obtained by applying the atanh
% formula to those indices of x corresponding to the ith
% row in the global matrix S_

    global S_ q
    
    y = ones(q,1);
    for i = 1:q
        for j=find(S_(i,:))
            y(i) = y(i)*tanh(x(j)/2);
        end
    end
    
    y = 2*atanh(y);
end