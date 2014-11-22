/*
Copyright (c) 2014 Joe Davisson.

This file is part of Rendera.

Rendera is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Rendera is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Rendera; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

#include "Octree.H"

Octree::Octree()
{
  root = new node_t;
  root->value = 0;

  for(int i = 0; i < 8; i++)
    root->child[i] = 0;
}

Octree::~Octree()
{
  clear(root);
}

void Octree::clear(struct node_t *node)
{
  for(int i = 0; i < 8; i++)
    if(node->child[i])
      clear(node->child[i]);

  delete node;
}

void Octree::write(const int &r, const int &g, const int &b,
                   const float &value)
{
  struct node_t *node = root;

  for(int mask = 128; mask != 0; mask >>= 1)
  {
    const int index = ((r & mask) ? 1 : 0) |
                      ((g & mask) ? 2 : 0) |
                      ((b & mask) ? 4 : 0);

    if(!node->child[index])
    {
      node->child[index] = new node_t;
      node = node->child[index];
      node->value = 0;

      for(int j = 0; j < 8; j++)
        node->child[j] = 0;
    }
    else
    {
      node = node->child[index];
    }
  }

  node->value = value;
}

// Sets entire path to value.
// This allows the octree to be used in a different way.
// (Needed for palette lookup.)
void Octree::writePath(const int &r, const int &g, const int &b,
                       const float &value)
{
  struct node_t *node = root;

  for(int mask = 128; mask != 0; mask >>= 1)
  {
    const int index = ((r & mask) ? 1 : 0) |
                      ((g & mask) ? 2 : 0) |
                      ((b & mask) ? 4 : 0);

    if(!node->child[index])
    {
      node->child[index] = new node_t;
      node = node->child[index];
      node->value = value;

      for(int j = 0; j < 8; j++)
        node->child[j] = 0;
    }
    else
    {
      node = node->child[index];
    }
  }
}

float Octree::read(const int &r, const int &g, const int &b)
{
  struct node_t *node = root;

  for(int mask = 128; mask != 0; mask >>= 1)
  {
    const int index = ((r & mask) ? 1 : 0) |
                      ((g & mask) ? 2 : 0) |
                      ((b & mask) ? 4 : 0);

    if(node->child[index])
      node = node->child[index];
    else
      break;
  }

  return node->value;
}

