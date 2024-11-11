# weak primality tests
function divisontest(n)
    println("n: ",n)
    foundwitness = false
    for i in range(2,n-1)
        if n % i == 0
            println("witnesses found: ",i)
            foundwitness = true
        end
    end
    if !foundwitness
        println("?")
    end
end
println("weak tests:")
println("1) division test:")
divisontest(11)
println()
println("2) fermat's test:")
function fermatstest(n)
    println("n: ",n)
    k = 10
    comp = false

    function gcdfunc(a,b)
        if a > b
            gcd(a-b,a)
        elseif a < b
            gcd(a,b-a)
        else
            return a
        end
    end

    function modexp(a,p,n)
        if n == 0
            return p
        else
            modexp(a,p*a,n-1)
        end
    end

    for i in range(1,k)
        
        a = rand(2:n)
        apowmodn = modexp(a,1,n-1) % n
        # println("gcd(a,n) = ",gcd(a,n)) this also works
        gcdan = gcdfunc(a,n)
        if apowmodn!=1
            if gcdan == 1
                println("test with a: ",a)
                println(a,"^(n-1) mod n = ",apowmodn)
                println("gcd(a,n) = ",gcdan)
                println("by the contrapositive of fermat's little theorem, n is composite with witness ",a)
                comp = true
            end
        #     else
        #         println("?")
        #     end
        # else
        #     println("?")
        end
    end
    if !comp
        println("?")
    end
end
fermatstest(11)
println()


println("3) using non trivial square roots:")
function nontrivialsquareroots(p,q)
    # function gcdfunc(a,b)
    #     if a > b
    #         gcd(a-b,a)
    #     elseif a < b
    #         gcd(a,b-a)
    #     else
    #         return a
    #     end
    # end
    n = p*q
    function coprime(n)
        a = rand(2:n)
        while gcd(a,n) != 1
            a = rand(2:n)
        end
        return a
    end
    a = coprime(n)
    phin = (p-1)*(q-1)
    e = rand(2:phin)
    while gcd(e,phin) != 1
        e = rand(2:phin)
    end
    d = invmod(e,phin)
    println("e: ",e," d: ",d," phin: ",phin)
    maxs = 1
    while 2^maxs <= e*d - 1
        maxs = maxs + 1
    end
    maxs = maxs - 1
    s = 0
    for i in range(1,maxs)
        if (e*d - 1)%2^i == 0
            s = i
        end
    end
    t = div((e*d - 1),2^s)
    println("e*d - 1 = ",e*d - 1)
    println("2^(",s,")",t," = ","e*d - 1")
    witnessfound = false
    while true
        global counter = 0
        global countbreak = false
        a = coprime(n)
        global b = a^t % (n)
        while b^2 % (n) != 1
            global b = b^2 % (n)
            global counter = counter + 1
            if counter == 15
                break
            end
        end
        if b % (n) != 1 && b%(n) != -1 && b^2 % (n) == 1
            break
        end
    end
    if countbreak
        println("countbreak: ",countbreak)
    end
    println("a: ",a)
    println("b = a^(2^(",counter,")",t,") = ",b)
    println("The attacker now has access to a non-trivial square root ",b," of 1 mod ",n)
    println("As a result, the factor gcd(b-1,n) = ",gcd(b-1,n)," and the factor div(n,gcd(b-1,n)) = ",div(n,gcd(b-1,n))," of ",n," have been exposed")
end
println("n = 89*179")
nontrivialsquareroots(89,179)
println()

function millerrabin(n)
    maxs = 1
    while 2^maxs <= n - 1
        maxs = maxs + 1
    end
    maxs = maxs - 1
    s = 0
    for i in range(1,maxs)
        if (n - 1)%2^i == 0
            s = i
        end
    end
    t = div((n - 1),2^s)
    println("2^(",s,")",t," = ","n - 1")
    k = 1024 # number of rounds
    for i in range(1,k)
        a = rand(2:n-2)
        global x = a^t % n
        for i in range(1,s)
            global y = x^2 % n
            if y == 1 && x!=1 && x!=-1
                println(x," is a non-trivial square root of 1 mod ",n)
                println(n," is composite")
                return
            end
            x = y
        end
        if y!=1 && gcd(x-1,n)!=1 && gcd(x+1,n)!=1
            println(n," is composite by contrapositive of fermat's little theorem and ",gcd(x-1,n)," is a factor")
            return
        end
    end
    println("?")
end
println("Miller Rabin:")
println("n = ",89*179)
millerrabin(89*179)