import zlib
import os
import sys

class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()
        self.branch_name = set()
        self.in_degree = 0

def topo_order_commits():
    cur = os.getcwd()
    found = False
    while not found:
        dirs = os.listdir(cur)
        for f in dirs:
            if f == ".git":
                found = True
                break
        if not found and cur == "/":
            print("Not inside a Git repository", file=sys.stderr)
            exit(1)
        if not found:
            head_tail = os.path.split(cur)
            cur = head_tail[0]

    # find branch_name in .git/refs/heads
    branch_dir = os.path.join(cur, ".git/refs/heads")
    branch = os.listdir(branch_dir)

    branches = {}
    all_node = {}
    root_commits = set()
    stack = []

    def create_node(d):
        # create or retrieve a node
        while len(stack) > 0:
            commit_hash = stack[-1]
            stack.pop()
            if commit_hash not in all_node.keys():
                temp = CommitNode(commit_hash)
                all_node[commit_hash] = temp
            else:
                temp = all_node[commit_hash]
            # decompress and find its parent
            fir = commit_hash[:2]
            sec = commit_hash[2:]
            hash_loc = os.path.join(d, fir, sec)
            compressed = open(hash_loc, 'rb').read()
            content = zlib.decompress(compressed).decode()
            store = content.split("\n")
            for s in store:
                if s[:6] == "parent":
                    temp.parents.add(s[7:])
            if len(temp.parents) == 0:
                root_commits.add(temp.commit_hash)
            else:
                for p in temp.parents:
                    stack.append(p)

    for f in branch:
        file = os.path.join(branch_dir, f)
        hash_num = open(file).read()[:-1]
        if hash_num not in branches:
            branches[hash_num] = {f}
        else:
            branches[hash_num].add(f)
        # for every branch, trace back its parents and add children
        obj_dir = os.path.join(cur, ".git/objects")
        stack.append(hash_num)
        create_node(obj_dir)

    # add child and branch names
    for sha_id in all_node:
        for parent in all_node[sha_id].parents:
            all_node[sha_id].in_degree +=1
            all_node[parent].children.add(sha_id)
    for sha_id in branches.keys():
        all_node[sha_id].branch_name = branches[sha_id]

    res_du = [] # duplicated version of topological sort

    def dfs():
        queue = []
        for r in sorted(root_commits):
            queue.append(all_node[r])

        while queue:
            node = queue.pop()
            res_du.insert(0, node.commit_hash)

            for c in node.children:
                all_node[c].in_degree -= 1
                # If in-degree becomes zero, add it to queue
                if all_node[c].in_degree == 0:
                    queue.append(all_node[c])

    # DFS to generate a topological ordering
    dfs()

    # remove duplicates
    res = []
    [res.append(x) for x in res_du if x not in res]

    # print out the commands
    def print_commit(com):
        print(com, end="")
        for b in sorted(all_node[com].branch_name):
            print(" " + b, end="")
        print("")

    def print_parents(comm):
        if len(all_node[comm].parents) > 0:
            for p in list(all_node[comm].parents)[:-1]:
                print(p, end=" ")
            print(list(all_node[comm].parents)[-1], end="")
        print("=")

    def print_child(commi):
        print("=", end="")
        if len(all_node[commi].children) > 0:
            for c in list(all_node[commi].children)[:-1]:
                print(c, end=" ")
            print(list(all_node[commi].children)[-1], end="")
        print("")

    switch_b = False
    i = 0
    for commit in res:
        # if last one to output
        if i == len(res) - 1:
            # if this one is a new branch
            if res[i - 1] not in all_node[commit].children:
                print_child(commit)
            print_commit(commit)
            i = i + 1
            continue
        # if this one to output is in a new branch
        if switch_b:
            # print its children with sticky starts; if none, just print a "="
            print_child(commit)
            print_commit(commit)
            switch_b = False
            # check if next one to output is a new branch
            if res[i + 1] not in all_node[commit].parents:
                print_parents(commit)
                print("")
                switch_b = True
            i = i + 1
            continue
        # if this one and next one to output is still in the same branch
        if res[i + 1] in all_node[commit].parents:
            print_commit(commit)
            i = i + 1
            continue
        # if next one to output is in a new branch
        elif res[i + 1] not in all_node[commit].parents:
            print_commit(commit)
            print_parents(commit)
            print("")
            switch_b = True
            i = i + 1


if __name__ == '__main__':
    topo_order_commits()