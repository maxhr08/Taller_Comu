function [G] = generatormatrix(H)
%given a sparse parity check matrix, H compute a generator matrix G

    Hp = H;
    [m,n] = size(Hp);

    %suppose m<n!

    colperm = 1:n;

    for j=1:m
        %find row to put a new row j
        i = find(Hp(j:m,j),1);
        if isempty(i)
            %do some column swapping
            k=min(max(find(Hp(j,:)),j));
            if isempty(k)
                disp(['probmen in row' num2str(j,0)]);
                continue;
            end
            temp = Hp(:,j);
            Hp(:,j) = Hp(:,k);
            Hp(:,k) = temp;
            temp = colperm(k);
            colperm(k) = colperm(j);
            colperm(j) = temp;
        end
        %swap rows
        %adjust indeces
        i=i+j-1;
        if (i~=j)
            temp = Hp(j,:);
            Hp(j,:) = Hp(i,:);
            Hp(i,:) = temp;
        end
        %clear out rest of column
        K = find(Hp(:,j));
        K = K(find(K~=j));
        if ~isempty(K)
            t1 = full(Hp(j,:));
            for k=K'
                t2 = full(Hp(k,:));
                temp = xor(t1,t2);
                Hp(k,:) = sparse(temp);
            end
        end
    end
    
    %now Hp = [Id_m A]
    A = Hp(: ,m+1:n);
    
    %Compute G
    [b, invperm] = sort(colperm);
    G = [A; speye(n-m)];
    G = G(invperm,:);
    
    %consistency check: mod(H*G,2) should be all zero matrix
    mod(H*G,2)
end

    
    