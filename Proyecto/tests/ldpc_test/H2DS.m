function[y]=H2DS(H,T,u)
global B P S_ q m n

[m,n]=size(H) ;
q=nnz(H);
P=spalloc(q,q,(sum(H,2)-1)'*sum(H,2)) ;
S_=spalloc(q,q,(sum(H,1)-1)*sum(H,1 )');
k=0;
for j=1:n
   I=find(H( : ,j)); 
   for x=1:length(I) 
       for y=x+1:length(I) 
          P(k+x,k+y)=1;
          P(k+y,k+x)=1; 
       end    
   end
   k=k+length(I);
end
k=0;
for i=1:m
    J=find(H(i, :));
    for x=1:length(J)
        for y=x+1:length(J)
            S_(k+x,k+y)=1;
            S_(k+y,k+x)=1;
        end
    end
    k=k+length(J);
end

B=spalloc(q,n,q);
b=[];
for k=1:m
    b = [b find(H(k,:))] ;
end
B=sparse([1:q]',b',ones(q,1),q,n);

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