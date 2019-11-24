function H = generate_H(m,n,d)
% H = GENERATE_H (m,n,d)
%
% generate a m by n parity check matrix, where the density is
% influenced by the parameter d (between 0 and 1)

    H = sparse(m,n);
    H = mod(H,2);

    while not(all(sum(H,1)>=2) && all(sum(H,2)>=2))
        H = H+abs(sprand(m,n,d))>0;
        H = mod(H,2);
    end
end