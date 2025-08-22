#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

typedef struct {
    char* str;
    int size;
    int capacity;
} StringBuilder;

void initStringBuilder(StringBuilder* sb) {
    sb->size = 0;
    sb->capacity = 16;
    sb->str = malloc(sb->capacity);
    sb->str[0] = '\0';
}

void appendString(StringBuilder* sb, char* s) {
    int len = strlen(s);

    if (sb->size + len + 1 > sb->capacity) {
        sb->capacity *= 2;
        sb->str = realloc(sb->str, sb->capacity);
    }

    strcpy(sb->str + sb->size, s);
    sb->size += len;
}

void serializeHelper(struct TreeNode* node, StringBuilder* sb) {
    if (!node) {
        appendString(sb, "null,");
        return;
    }

    char buffer[20];

    sprintf(buffer, "%d,", node->val);
    appendString(sb, buffer);
    serializeHelper(node->left, sb);
    serializeHelper(node->right, sb);
}

char* serialize(struct TreeNode* root) {
    StringBuilder sb;
    initStringBuilder(&sb);
    serializeHelper(root, &sb);

    char* result = strdup(sb.str);

    free(sb.str);

    return result;
}

struct TreeNode* deserializeHelper(char** tokens, int* index, int tokenCount) {
    if (*index >= tokenCount || strcmp(tokens[*index], "null") == 0) {
        (*index)++;
        return NULL;
    }

    struct TreeNode* node = malloc(sizeof(struct TreeNode));

    node->val = atoi(tokens[(*index)++]);
    node->left = deserializeHelper(tokens, index, tokenCount);
    node->right = deserializeHelper(tokens, index, tokenCount);

    return node;
}

struct TreeNode* deserialize(char* data) {
    char* tokens[1000];
    int tokenCount = 0;
    char* token = strtok(data, ",");

    while (token) {
        tokens[tokenCount++] = token;
        token = strtok(NULL, ",");
    }

    int index = 0;

    return deserializeHelper(tokens, &index, tokenCount);
}

void printInorder(struct TreeNode* root) {
    if (!root) return;
    printInorder(root->left);
    printf("%d ", root->val);
    printInorder(root->right);
}

void freeTree(struct TreeNode* root) {
    if (!root) return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

const int main(void) {
    struct TreeNode* root = malloc(sizeof(struct TreeNode));
    root->val = 1;
    root->left = malloc(sizeof(struct TreeNode));
    root->left->val = 2;
    root->left->left = NULL;
    root->left->right = NULL;
    root->right = malloc(sizeof(struct TreeNode));
    root->right->val = 3;
    root->right->left = malloc(sizeof(struct TreeNode));
    root->right->left->val = 4;
    root->right->left->left = NULL;
    root->right->left->right = NULL;
    root->right->right = malloc(sizeof(struct TreeNode));
    root->right->right->val = 5;
    root->right->right->left = NULL;
    root->right->right->right = NULL;

    char* serialized = serialize(root);
    printf("Serialized: %s\n", serialized);

    struct TreeNode* deserialized = deserialize(serialized);
    printf("Deserialized Inorder: ");
    printInorder(deserialized);
    printf("\n");

    free(serialized);
    freeTree(root);
    freeTree(deserialized);

    return 0;
}
