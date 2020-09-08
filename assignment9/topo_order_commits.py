# Keep the function signature,
# but replace its body with your implementation.
#
# Note that this is the driver function.
# Please write a well-structured implemention by creating other functions outside of this one,
# each of which has a designated purpose.
#
# As a good programming practice,
# please do not use any script-level variables that are modifiable.
# This is because those variables live on forever once the script is imported,
# and the changes to them will persist across different invocations of the imported functions.
import os
import sys
import zlib


def topo_order_commits():
    git_directory = step_1() + "/.git/"
    head_to_branches = step_2(git_directory)
    commit_nodes, root_hashes = step_3(git_directory, head_to_branches)
    topo_ordered_commits = step_4(commit_nodes, root_hashes)
    step_5(commit_nodes, topo_ordered_commits, head_to_branches)

if __name__ == '__main__':
    topo_order_commits()

class CommitNode:
    def __init__(self, commit_hash, branch=[]):
        '''
        :type commit_hash: str 
        :type branch: list 
        '''
        self.commit_hash = commit_hash
        self.branch = branch
        self.parents = set()
        self.children = set()

def step_1():
    dir = "."
    while(os.path.abspath(dir) != "/"):
        for i in os.listdir(dir):
            if os.path.isdir(os.path.join(dir,i)):
                if i == ".git": 
                    return os.path.abspath(dir)
        dir = "../" + dir
    sys.stderr.write("Not inside a git repository")
    sys.exit(1)

def step_1_2(): 
    cwd = os.getcwd()
    direct = ""
    while True: 
        alRight = os.listdir(cwd)
        parDir = os.path.dirname(cwd)
        if ".git" in allRight and os.path.isdir(cwd + '/.git'): 
            if ".git" not in allRight: 
                break
            direct = cwd 
            break 
        else: 
            if cwd is parDir: 
                sys.stderr.write("Not inside a git repository")
                exit(1) 
            else: 
                cwd = parDir


def step_2(git_path):
    loc_br_path = git_path + "refs/heads/"
    br_dict = {}
    if os.path.isdir(loc_br_path) != True:
        sys.stderr.write("Error: Directory DNE\n")
        sys.exit(1)
    else: 
        for dirpath, dirnames, files in os.walk(loc_br_path):
            if files:
                for file in files:
                    commit_hash = open(os.path.join(dirpath, file), 'rb').read().decode("latin-1")
                    commit_hash = commit_hash.strip('\n')
                    if br_dict.get(commit_hash) == None: 
                        br_dict[commit_hash] = []
                    br_dict[commit_hash].append(os.path.join(dirpath, file)[len(loc_br_path):])
    return br_dict

def step_3(git_directory, local_branch_heads):
    commit_nodes = {} 
    root_hashes = set()
    obj = git_directory + "objects/"
    for commit_hash in local_branch_heads:
        if commit_nodes.get(commit_hash) == None:
            commit_nodes[commit_hash] = CommitNode(commit_hash, local_branch_heads[commit_hash])
            stack = [commit_nodes[commit_hash]] 
            root_hashes2 = set()
            visited = set()
            while len(stack) > 0:
                top = stack.pop()
                parent_list = set()
                commit_path = obj + top.commit_hash[:2] + "/" + top.commit_hash[2:]
                if os.path.isfile(commit_path):
                    data = {}
                    data = step_3_decompress(commit_path, data)
                    i = 0
                    while i < len(data) and data[i] != "author":
                        if data[i-1] == "parent":
                            parent_list.add(data[i])
                        i = i + 1
                else:
                        sys.stderr.write(top.commit_hash)
                parent_list = sorted(parent_list) 
                top.parents = parent_list
                if len(top.parents) == 0:
                    root_hashes2.add(top.commit_hash)
                else:
                    for parent_hash in top.parents:
                        if commit_nodes.get(parent_hash) == None:
                            commit_nodes[parent_hash] = CommitNode(parent_hash)
                            parent_node = commit_nodes[parent_hash]
                            parent_node.children.add(top.commit_hash)
                            stack.append(parent_node)
                        else:
                            parent_node = commit_nodes[parent_hash]
                            parent_node.children.add(top.commit_hash)
            root_hashes = root_hashes.union(root_hashes2)
        else:
            commit_nodes[commit_hash].branch = local_branch_heads[commit_hash]
    root_hashes = sorted(root_hashes) 
    return commit_nodes, root_hashes

def step_3_decompress(commit_path, data): 
    object1 = open(commit_path, 'rb').read()
    object1 = zlib.decompress(object1).decode("latin-1")
    object1 = object1.replace("\n", " ")
    data = object1.split(" ")
    return data

def step_4_3(commit_nodes, root_hashes):
    order = []
    visited = set()
    temp_stack = []
    stack = sorted(root_hashes)
    while stack: 
        v = stack.pop()
        if v in visited:
            temp_stack.append(v)
        visited.add(v)
        #toppy = temp_stack[-1]
        while v is not commit_nodes[temp_stack[-1]].children: 
            g = temp_stack.pop()
            order.append(g)
        temp_stack.append(v)
        for c in sorted(commit_nodes[v].children):
            pop_top = stack.pop()
            order.append(pop_top)
            stack.append(c)
        l
        return order


def step_4(commit_nodes, root_hashes):
    visited = set()
    processed = set()
    order = [] 
    for i in sorted(list(root_hashes)):
        if i in visited: 
             continue 
        stack = [i]
        visited = set()
        while stack: 
            v = stack.pop()
            if v in processed: 
                if v not in order: 
                    order.append(v)
                continue 
            visited.add(v)
            processed.add(v)
            stack.append(v)
            for child in sorted(list(commit_nodes[v].children)):
                if child not in visited: 
                    stack.append(child)
    return order 

'''def step_4_2(commit_nodes, root_hashes):
    order = []
    visited = {}
    stack = list(root_hashes.copy())
    temp_stack = 0;
    while len(stack):
        if temp_stack == 1: 
            temp_stack = 0
        top = stack[-1]
        visited[top] = "already visited"
        unvisited = [i for i in commit_nodes[top].children if visited.get(i) == None]
        if len(unvisited) > 0:
            stack.append(unvisited[0])
        else:
            pop_top = stack.pop()
            order.append(pop_top)
    return order
'''

def step_5(commit_nodes, topo_ordered_commits, head_to_branches):
    jumped = False
    for i in range(len(topo_ordered_commits)):
        commit_hash = topo_ordered_commits[i]     
        if jumped:
            jumped = False
            sticky_hash = ' '.join(commit_nodes[commit_hash].children)
            print(f'={sticky_hash}')
        branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else []
        print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
        if i + 1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
            jumped = True
            sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
            print(f'{sticky_hash}=\n')



