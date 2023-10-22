/*
    Title:              Cherry Pickup
    Difficulty:         Hard
    Source:             https://leetcode.com/problems/cherry-pickup/
    Prerequisites:      https://leetcode.com/problems/unique-paths-ii/ 



    This type of problem, where we only move down and right in a 2D grid, is a 
    classic DP problem. Here, however, we not only travel one path, but two. 
    This complicates the problem a lot because depending on the first path,
    our grid state changes before we get to apply the second path.
    
    It's not immediately obvious how we can make a solution in polynomial time.
    That's because there are (n!) number of first paths we can travel, and
    a corresponding factorial number of "states" as a result. Then even if we
    can apply the prerequisite problem's DP method to the second path and find
    a solution in n^2 time, the factorial overhead is still there.
    
    The greedy first path is not necessarily valid as a solution. Furthermore, it
    appears that storing any N best number of first paths is also not viable,
    unless N captures all possible ones, at which point it's just brute force.

    We can make some observations that let us think about the problem from
    a different angle:

    1)  For any grid input, we can determine the number of steps required to 
        reach the end as (n*2 + 1), because we are only allowed to travel down
        and right.
    2)  The second path, where we backtrack from the end to the start, is 
        equivalent to travelling from start to end. (For simplicity)
    3)  At the m-th step from the origin, we have a subset of the grid that the 
        current position MUST reside on. For instance, if we take (0,0) to be the
        origin, then after one step we must be in one of {(1,0), (0,1)}. After
        two steps we must be in one of {(2,0), (1,1), (0,2)}. We can generalize 
        that at the m-th step, our coordinates (row, col) must sum to that step
        number. If you look at it visually, then the possible squares for a 
        given step will form a diagonal line from (m, 0) to (0, m).
    4)  If we are at any given square (r, c) then the possible next steps are
        (r+1, c) or (r, c+1).

    This allows us to tackle the problem in smaller steps. Basically, observation 2
    tells us that it doesn't matter which order we travel, because we always have
    to pass a predefined number of squares anyways (from observation 1). Then 
    we can traverse the grid just ONCE, starting from the origin, while keeping
    track of both the first path and second path.

    Doing it this way, we can treat the problem as if we are sliding down diagonals
    (from observation 3) rather than squares. Then it becomes just like the prerequisite
    problem, in that we travel once from top-left to bottom-right of the grid, and
    then we're done.

    At a given square (r, c), we arrive from DP[previous best path] which stores
    the best sum from either (r-1, c) or (r, c-1). We don't care which of those 
    previous steps were used, we only care about the best sum. Likewise, at a given
    pair of first path square and second path square, we arrive from
    DP[previous best first path][previous best second path]. We can keep the DP
    matrix in 2 dimensions (for n^2 time complexity) because we progress step-wise,
    so from observation 3, we know that we can store a pair of coordinates as one
    number. Therefore, we can store two coordinates (aka. a pair of first and second
    paths) as two numbers, so our DP array has two dimensions. (For instance, if 
    we are at step 4 and we know the row number is 3, then the column number must
    be 1, because 4-3 = 1.)

    (In my code below, we start indexing steps from 0 because the origin is not
    a "step", hence I put "--steps"... but you get my point.)

    Finally, since we are progressing step-wise, we only need to keep a DP
    of the last step to find the DP of the current step. This lets us keep space
    complexity down. At the end of calculating each step, we swap the current and
    next DP vectors (which basically swaps the data pointers internally).

    Other implementation details are checking for bounds, THORN (we can't progress).

*/



class Solution {

enum {
    EMPTY = 0,
    CHERRY = 1,
    THORN = -1
};

public:
    const int dirs[2][2] {{1, 0}, {0, 1}};

    int cherryPickup(vector<vector<int>>& mat) {
        int rows = mat.size(), cols = mat[0].size(), steps = rows+cols-1;
        auto inBounds = [&](int r, int c) {
            return (r >= 0 && r < rows && c >= 0 && c < cols);
        };

        --steps; // dont consider {0,0}

        vector<vector<int>> dp(rows, vector<int>(cols, INT_MIN)), next(dp);
        dp[0][0] = (mat[0][0] == CHERRY) ? CHERRY : (mat[0][0] == THORN) ? THORN : EMPTY;

        for (int s = 0; s < steps; ++s) {
            bool somePath = false;
            // get all current paths
            for (int r1 = 0; r1 < rows && r1 <= s; ++r1) {
                for (int r2 = 0; r2 < rows && r2 <= s; ++r2) {
                    int c1 = s-r1, c2 = s-r2;
                    if (dp[r1][r2] < 0) continue;
                    /* NOT NECESSARY to check (!inBounds(r1, c1) || !inBounds(r2, c2) 
                        since r1 goes from [0] to [rows or s, whichever is lower].  */
                    
                    // get all next paths
                    for (int i = 0; i < 2; ++i) {
                        for (int j = 0; j < 2; ++j) {
                            int r3 = r1 + dirs[i][0], c3 = c1 + dirs[i][1];
                            int r4 = r2 + dirs[j][0], c4 = c2 + dirs[j][1];
                            // {r3, c3} denote next step for {r1, c1}. (And vice versa for even numbers)
                            if (inBounds(r3, c3) && inBounds(r4, c4) && mat[r3][c3] != THORN && mat[r4][c4] != THORN) {
                                somePath = true;
                                int newCherries = max(0, mat[r3][c3] + mat[r4][c4] - (r3 == r4 && c3 == c4));
                                next[r3][r4] = max(next[r3][r4], dp[r1][r2] + newCherries);
                            }
                        }
                    }

                }
            }
            if (!somePath) return false;
            dp.swap(next);
            next.assign(rows, vector<int>(cols, INT_MIN));
        }

        return max(0, dp[rows-1][cols-1]);
    }
};