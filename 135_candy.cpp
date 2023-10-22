/*
    Title:              Candy
    Difficulty:         Hard
    Source:             https://leetcode.com/problems/candy/


    This problem involves trying to minimize the number of candies that we hand 
    out. Let's start off with some observations. Firstly, if our entire ratings
    array is strictly increasing, then we are forced to hand out 1 + 2 + ... + n
    candies, where n is the size of our array. The same logic applies if our 
    array is strictly decreasing. In these two cases, the equation for the summation
    from 1 to n is (n)(n+1)/2 candies. If our array consists of one repeated number,
    then we hand out 1 + 1 + ... + 1 candies, for a total of n candies.

    Now going further off the idea of a steadily increasing array, we see that
    we are forced to keep increasing our candy handouts because the children are
    all neighboring with a lower neighbor. In other words, the only reason why we'd 
    give out more than 1 candy is because our child is rated higher than an adjacent
    child.

    How do we give the least candy out? Well, whenever we have a strictly increasing
    or decreasing subarray within our bigger array, we can fix the lowest point 
    to 1 candy (the minimum). Then the highest rated child will receive (len) candies,
    where len is the length of the subarray in question. From above, this is because
    we must hand out 1 + 2 + ... + (len) candies.

    Now we can subdivide our array into these strictly monotonic subsequences 
    (I call them "UP", "DOWN", and "FLAT", pretty self-explanatory). We can make
    one pass through our input array, storing the current slope that we're riding
    on. If the slope changes, then we know we've reached the maximum length of that
    subarray, and we can immediately process it and add to our solution.

    The main trick to this problem, when stitching together these different subarrays,
    is to overlap them with one another. This means that they share endpoints.
    If we store both of these endpoints (in code, denoted "lastVal" and "newVal"),
    then we know the solution will double-count both of them, even though both
    are from the same index in the input array. However, in reality, we cannot
    double-count, so which one do we remove? Well, recall that we want to save candies.
    Do we simply remove the max? No, we cannot, because the max exists due to a 
    condition that we already verified (we are forced to hand out "len" candies,
    for instance). This means we are forced to hand out the greater of the two:""
    because the two are each the minimum needed for each individual subarray, that
    means to satisfy both sides, we must give out the max. That means we double-counted
    the min of the two, so we subtract that from our solution.

    Also, instead of branching statements in the code, I squeezed some better runtime 
    out of it by using boolean algebra. For instance:

    "int newVal = (lastSlope == DOWN) * len + (lastSlope != DOWN) * 1;"
    
    logically means

    "int newVal = (lastSlope == DOWN) ? len : 1;"

    I thought I'd leave it in the more confusing but faster way so you could try 
    for yourself the speed difference you get (I got roughly 25%). 



*/


class Solution {
    enum {
        UP = 0,
        DOWN = 1,
        FLAT = 2
    };

public:
    int candy(vector<int>& rates) {
        int soln = 0, sz = rates.size(), leftIdx = 0, lastVal = 0, lastSlope = 0;

        /* Idea: distinguish each strictly increasing section .
            Each section has min point fixed to 1 */
        for (int i = 1; i < sz; ++i) {
            int slope;
            if (rates[i] > rates[i-1]) 
                slope = UP;
            else if (rates[i] < rates[i-1]) 
                slope = DOWN; 
            else 
                slope = FLAT;

            // border case left edge. Guarantee this slope extends past left edge
            if (i == 1) 
                lastSlope = slope;

            if (lastSlope != slope) {
                int len = i - leftIdx;
                int newVal = (lastSlope == DOWN) * len + (lastSlope != DOWN) * 1;
                soln += (lastSlope == FLAT) * len + (lastSlope != FLAT) * (len*(len+1) / 2);
                soln -= min(lastVal, newVal); // fix double-count. We need the max, so uncount min.

                lastVal = (lastSlope == UP) * len + (lastSlope != UP) * 1;
                leftIdx = i-1; // force an overlap and double count
            }
            lastSlope = slope;
        }

        /* clean up last section */
        int len = sz - leftIdx;
        int newVal = (lastSlope == DOWN) * len + (lastSlope != DOWN) * 1;
        soln += (lastSlope == FLAT) * len + (lastSlope != FLAT) * (len*(len+1) / 2);
        soln -= min(lastVal, newVal); // fix double-count. We need the max, so uncount min.

        return soln;
    }
};