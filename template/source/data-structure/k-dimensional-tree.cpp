const int N = ;

struct Point {
    int x, y, id;
};

long long sqr(const long long &x) {
    return abs(x);
}

long long dist(const Point &a, const Point &b) {
    return sqr(a.x - b.x) + sqr(a.y - b.y);
}

struct Rectangle {
    int lx, rx, ly, ry;
    
    void set(const Point &p) {
        lx = rx = p.x;
        ly = ry = p.y;
    }
    void mergy(const Point &p) {
        lx = min(lx, p.x);
        rx = max(rx, p.x);
        ly = min(ly, p.y);
        ry = max(ry, p.y);
    }
    void mergy(const Rectangle &r) {
        lx = min(lx, r.lx);
        rx = max(rx, r.rx);
        ly = min(ly, r.ly);
        ry = max(ry, r.ry);
    }
    /*    Minimum distance    */
    long long dist(const Point &p) {
        if (p.x <= lx && p.y <= ly) {
            return sqr(p.x - lx) + sqr(p.y - ly);
        }
        if (p.x <= rx && p.y <= ly) {
            return sqr(p.y - ly);
        }
        if (p.x >= rx && p.y <= ly) {
            return sqr(p.x - rx) + sqr(p.y - ly);
        }
        if (p.x >= rx && p.y <= ry) {
            return sqr(p.x - rx);
        }
        if (p.x >= rx && p.y >= ry) {
            return sqr(p.x - rx) + sqr(p.y - ry);
        }
        if (p.x >= lx && p.y >= ry) {
            return sqr(p.y - ry);
        }
        if (p.x <= lx && p.y >= ry) {
            return sqr(p.x - lx) + sqr(p.y - ry);
        }
        if (p.x <= lx && p.y >= ly) {
            return sqr(p.x - lx);
        }
        return 0;
    }
    /*    Maximum distance    */
    long long dist(const Point &p) {
        long long ret = 0;
        ret += max(sqr(rx - p.x), sqr(lx - p.x));
        ret += max(sqr(ry - p.y), sqr(ly - p.y));
        return ret;
    }
};

struct Node {
    int child[2];
    Point p;
    Rectangle r;
    
    void set(const Point &p) : p(p) {
        r.set(p);
        child[0] = child[1] = 0;
    }
};

int size;
Point a[N];
Node tree[N];

bool xcompare(const Point &a, const Point &b) {
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

bool ycompare(const Point &a, const Point &b) {
    return a.y < b.y || a.y == b.y && a.x < b.x;
}

int build(int left, int right, bool dim = 0) {
    int x = ++size, mid = left + right >> 1;
    nth_element(a + left, a + mid, a + right, dim ? xcompare : ycompare);
    tree[x].set(a[mid]);
    if (left < mid) {
        tree[x].child[0] = build(left, mid, dim ^ 1);
        tree[x].r.mergy(tree[tree[x].child[0]].r);
    }
    if (mid + 1 < right) {
        tree[x].child[1] = build(mid + 1, right, dim ^ 1);
        tree[x].r.mergy(tree[tree[x].child[1]].r);
    }
    return x;
}

int insert(int x, const Point &p, bool dim = 0) {
    if (x == 0) {
        tree[++size].set(p);
        return size;
    }
    tree[x].r.mergy(p);
    if (dim && xcompare(p, tree[x].p) || !dim && ycompare(p, tree[x].p)) {
        tree[x].child[0] = insert(tree[x].child[0], p, dim ^ 1);
    } else {
        tree[x].child[1] = insert(tree[x].child[1], p, dim ^ 1);
    }
    return x;
}

/*    Query minimum    */
void query(int x, const Point &p, long long &ret, bool dim = 0) {
    if (tree[x].r.dist(p) >= ret) {
        return;
    }
    ret = min(ret, dist(tree[x].p, p));
    if (dim && xcompare(p, tree[x].p) || !dim && ycompare(p, tree[x].p)) {
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, ret, dim ^ 1);
        }
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, ret, dim ^ 1);
        }
    } else {
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, ret, dim ^ 1);
        }
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, ret, dim ^ 1);
        }
    }
}

/*    Query maximum    */
void query(int x, const Point &p, long long &ret, bool dim = 0) {
    if (tree[x].r.dist(p) <= ret) {
        return;
    }
    ret = max(ret, dist(tree[x].p, p));
    if (dim && xcompare(p, tree[x].p) || !dim && ycompare(p, tree[x].p)) {
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, ret, dim ^ 1);
        }
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, ret, dim ^ 1);
        }
    } else {
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, ret, dim ^ 1);
        }
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, ret, dim ^ 1);
        }
    }
}

/*    Query kth-minimum    */
void query(int x, const Point &p, int k, pair<long long, int> ret[], bool dim = 0) {
    if (tree[x].r.dist(p) > ret[k].first) {
        return;
    }
    pair<long long, int> val = make_pair(dist(tree[x].p, p), tree[x].p.id);
    for (int i = 1; i <= k; ++i) {
        if (val < ret[i]) {
            for (int j = k + 1; j > i; --j) {
                ret[j] = ret[j - 1];
            }
            ret[i] = val;
            break;
        }
    }
    if (dim && xcompare(p, tree[x].p) || !dim && ycompare(p, tree[x].p)) {
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, k, ret, dim ^ 1);
        }
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, k, ret, dim ^ 1);
        }
    } else {
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, k, ret, dim ^ 1);
        }
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, k, ret, dim ^ 1);
        }
    }
}

/*    Query kth-maximum    */
void query(int x, const Point &p, int k, pair<long long, int> ret[], bool dim = 0) {
    if (tree[x].r.dist(p) < ret[k].first) {
        return;
    }
    pair<long long, int> val = make_pair(dist(tree[x].p, p), -tree[x].p.id);
    for (int i = 1; i <= k; ++i) {
        if (val > ret[i]) {
            for (int j = k + 1; j > i; --j) {
                ret[j] = ret[j - 1];
            }
            ret[i] = val;
            break;
        }
    }
    if (dim && xcompare(p, tree[x].p) || !dim && ycompare(p, tree[x].p)) {
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, k, ret, dim ^ 1);
        }
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, k, ret, dim ^ 1);
        }
    } else {
        if (tree[x].child[0]) {
            query(tree[x].child[0], p, k, ret, dim ^ 1);
        }
        if (tree[x].child[1]) {
            query(tree[x].child[1], p, k, ret, dim ^ 1);
        }
    }
}

void clear() {
    size = 0;
}