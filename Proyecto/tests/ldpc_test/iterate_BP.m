function y = iterate_BP(T,u)

    global B P S_ n m

    x1_k=zeros(q,1);
    x2_k=zeros(q,1);
    x1_k_1=zeros(q,1);
    x2_k_1=zeros(q,1);

    y=zeros(n,T+1);
    for t=1:T
        x1_k_1=P*x2_k+B*u;
        x2_k_1=S(x1_k) ;
        y(:,t)=B'*x2_k+u;
        x1_k=x1_k_1;
        x2_k=x2_k_1;
    end

    y(:,T+1)=B'*x2_k+u;

end