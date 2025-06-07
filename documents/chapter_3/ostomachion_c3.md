
# 3. Geometric representation

In the previous post, we figured out the algorithm to solve the Ostomachion puzzle. Looking ahead, we will need to figure out a way to efficiently store puzzle configurations in memory, how to quickly check if a configuration is valid or not and minimize memory transfer operations along the whole way.

A valid configuration consists of a frame and a set of polygons sharing edges and vertices. To produce the next valid configuration from this one we need to:

1. select an anchor vertex from the frame in this configuration
2. place an unused polygon on that anchor vertex. 
3. check if the new polygon overlaps any other polygons and is still inside the frame
4. if that was successful, then create a new configuration with this polygon added and an updated frame

Most of the computational effort will likely lie in the overlap checks while most of the memory operations will probably lie in the creation of a new configuration. With this in mind, these are a few solutions that could be used:

## Solution 1: keep a list of used polygons and their positions

This is the simplest approach. We just keep track of the positions of every polygon already placed, and each time that we want to add a new polygon, we just need to check if it overlaps any of the existing ones. If all the polygons involved are convex, we can use an efficient [separating-axis algorithm](https://web.archive.org/web/20141127210836/http://content.gpwiki.org/index.php/Polygon_Collision) to check for overlap. If they’re not, we can divide each non-convex polygon into a disjoint set of convex polygons, such that each check is simpler. If there are a lot of non-convex polygons, this quickly becomes another combinatorial nightmare. 

Alternatively, it’s possible to check if two polygons overlap by introducing additional checks. To be completely thorough, a lot of checks need to be done (and we’ll discuss these at length later in this), but realistically, most of the times that two polygons overlap, some edges will intersect. There are edge cases where this is not enough, and that’s why we require additional verifications, but these happen seldom enough that this general approach is still efficient enough.

The downsides of this solution is that the more polygons exist in the configuration, the more checks need to be made, so the algorithm becomes slower precisely in sections of the search tree where there’s a lot of configurations. It also requires storing the locations of every vertex - adding memory transfer overhead when creating new configurations. It also has redundancy, because the same edge may be part of two different polygon, and it will be checked twice.

## Solution 2: same as above, but add bounding boxes

Taking inspiration from game design, adding a bounding box to each polygon reduces the amount of overlaps to check. A check for polygon overlap won’t even happen if the bounding boxes don’t overlap - and it’s very easy to check this. Additionally, organizing the polygons in a quad-tree can even reduce the amount of bounding box overlap checks. But each of these additional layers of optimization require additional data to be stored/transferred and more complicated logic and indirection. I highly doubt this is optimal when the objects we want to check are mostly triangles and when AVX vectorization can make checking intersections blazingly fast. I classify this solution as overengineering.

## Solution 3: merge and prune

A more elegant approach consists of representing the polygons as if they were patches of fabric which get sown together along their edges. The only thing that matters is the outer edge of the sown-together polygon, which can now be interpreted as the frame for the next iteration. This polygon is in general a non-convex polygon but minimizes redundancies: each time that a new polygon is added, it only needs to be checked for overlap with the frame, nothing else. Moreover, considering that most of the times only an edge intersection algorithm needs to be executed to determine overlap, that makes this a good candidate for the most efficient solution. Furthermore, this reduces redundancy because no duplicate edges or vertices can exist. I call this the merge and prune algorithm because it consists of two steps: 

1. Bring the polygons together and connect them through their common vertices such that they become one single (albeit weird-looking) polygon
2. Remove overlapping edges to minimize redundancy

Computer Science majors might recognize the first part as the algorithm to merge two linked lists. 



Merging and pruning removes redundancy and has a small memory footprint, but most importantly, it seemed like a fun thing to implement. So this was my method of choice. This is how it works:

## Merge

Since we want to merge shapes together, assigning an orientation will make the process easier and turns the polygons into *linked lists*. Let’s consider the frame to have its edges oriented in a clockwise fashion and all the polygons that we want to insert to be oriented anti-clockwise. This choice has many advantages - as will become evident along this post - but most importantly, it will allow us to interpret the final shape as a clockwise-oriented frame after the algorithm is done. Further, let’s define the “outside” portion of a polygon to always lie to the right of its oriented edge. So in the case of the frame, this definition of outside corresponds to the region where we want to place the puzzle polygons. This allows us to claim that a polygon can be added only if it does not overlap with the frame, i.e. their inside regions do not overlap. 



<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1px;">
  <div style="flex: 1 1 200px; text-align: center;">
    <img src="images_b/frame_000.svg">
    <p>1. Starting configuration</p>
  </div>
  <div style="flex: 1 1 200px; text-align: center;">
    <img src="images_b/frame_002.svg">
    <p>2. Add a polygon</p>
  </div>
  <div style="flex: 1 1 200px; text-align: center;">
    <img src="images_b/frame_004.svg">
    <p>3. Merge the linked lists </p>
  </div>
</div>

<p style="text-align: center;"><strong>Figure 2:</strong> Inserting polygons inside the frame requires merging the two linked lists.</p>


## Prune

After merging, some of the edges will be overlapping, representing paths going back and forth which can be simplified. Some adjacent vertices might also be overlapping, in which case one of them gets removed. This process is repeated until the frame cannot be simplified further. At its limit, it will simplify down to the empty set when a solution has been found.

<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1px; ">
    <img src="images/output.gif" style="max-width: 500px">
</div>
<p style="text-align: center;"><strong>Figure 2:</strong> Stretch</p>

The resulting frame polygon is in general non-convex, so we need to come up with an algorithm that is able to detect when two general polygons overlap. This is the subject of the next section.

# When is the polygon entirely inside the frame?

This verification is at the core of solving the Ostomachion puzzle. We need to reliably be able to tell when a configuration of polygons is valid or not, which requires knowing when a polygon candidate will be entirely contained inside the frame.

The most obvious sign that the polygon is not inside the frame is when any of their its edges intersects with those of the frame. It means that some portion of it is inside and another is outside the frame, so it cannot be entirely contained in the frame. If no intersection occurs, we still need to check whether it is inside the frame or not. This is easily done with the ray tracing algorithm. The red polygons are invalid, the blue ones are valid.

<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1px;">
    <img src="c3c_edges_intersect.svg">
</div>
<p style="text-align: center;"><strong>Figure 2:</strong> Stretch</p>


These two situations cover most of the common polygon overlaps, that is, the ones that don’t involve any coincidences. When vertices coincide with other vertices or edges, things start getting interesting. Let’s analyze some cases where neither of the previous approaches could detect overlap.

## Edge-vertex coincidence

The figure on the left shows cases where polygon edges intersect with frame vertices and polygon vertices intersect frame edges. The figure on the right shows cases where polygon vertices intersect with frame edges but those create a coincident edge.


<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1px;">
  <div style="flex: 1 1 200px; text-align: center;">
    <img src="c3d_edges_vertex_intersect.svg">
    <p>1. Edges intersect vertices</p>
  </div>
  <div style="flex: 1 1 200px; text-align: center;">
    <img src="c3e_edges_vertex_intersect.svg">
    <p>2. Edges coincide</p>
  </div>
</div>

<p style="text-align: center;"><strong>Figure 2:</strong> First cases where a simple edge intersection algorithm would not work: when edges intersect vertices.</p>


Grabbing the frame and stretching it out into a straight line, what’s actually going on becomes clearer after drawing out all the arrows and focusing only on the vertices. The polygon and frame overlap whenever any vertex opens outwards or crosses the frame. If every vertex opens inwards, then the polygon and frame don’t overlap.


<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1px;">
    <img src="c3f_stretched.svg">
</div>
<p style="text-align: center;"><strong>Figure 2:</strong> Stretch</p>

Keeping in mind that the frame can also intersect the puzzle polygons’ edges, this verification needs to be done for both the puzzle polygon and the frame.

## Vertex-vertex coincidence

The last remaining case happens even less frequently: when vertices coincide. The next figure shows several examples that would not be captured by any of the previous approaches.

<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1px;">
    <img src="c3e_vertex_intersection.svg">
</div>
<p style="text-align: center;"><strong>Figure 2:</strong> Stretch</p>


Overlaps happen when the angle swept by the puzzle polygon overlaps with the angle swept by the corresponding vertex of the frame.

<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1px;">
    <img src="c3f_wedges.svg">
</div>
<p style="text-align: center;"><strong>Figure 2:</strong> Stretch</p>

