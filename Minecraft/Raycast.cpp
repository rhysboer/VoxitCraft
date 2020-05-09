#include "Raycast.h"
#include "World.h"
#include "Input.h"

#include "Renderer.h"

// Raycast from the center of the screen
Ray::RayHit Ray::RaycastFromCamera(const float& distance, BaseCamera& camera) {
	glm::vec2 screenSize = Engine::GetWindowSize();
	glm::vec2 mousePos = screenSize / 2.0f; 

	// Normalized Device Space
	glm::vec2 mouseNDS = glm::vec2((2.0f * mousePos.x) / screenSize.x - 1.0f, 1.0f - (2.0f * mousePos.y) / screenSize.y);

	// Eye Space														  * Clip Space
	glm::vec4 mouseEye = glm::inverse(camera.Projection()) * glm::vec4(mouseNDS, -1.0f, 1.0f);
	mouseEye.z = -1.0f;
	mouseEye.w = 0.0f;

	// World Space
	glm::vec3 rayDirection = glm::inverse(camera.View()) * mouseEye;
	rayDirection = glm::normalize(rayDirection);

	float cellSize = 1.0f;

	std::vector<glm::vec3> positions = std::vector<glm::vec3>();
	glm::vec3 startPoint = camera.GetPosition();
	glm::vec3 endPoint = camera.GetPosition() + (rayDirection * distance);    


	glm::vec3 startCell = glm::floor(startPoint); 
	glm::vec3 endCell = glm::floor(endPoint); 

	// Direction
	glm::vec3 direction = endPoint - startPoint;

	int step_X = (direction.x >= 0) ? 1 : -1;
	int step_Y = (direction.y >= 0) ? 1 : -1;
	int step_Z = (direction.z >= 0) ? 1 : -1;

	glm::vec3 norm_direction = glm::normalize(direction);

	// Distance to nearest square side
	double near_x = (step_X >= 0) ? ((startCell.x + 1) * cellSize - startPoint.x) : (startPoint.x - (startCell.x * cellSize)) ;
	double near_y = (step_Y >= 0) ? ((startCell.y + 1) * cellSize - startPoint.y) : (startPoint.y - (startCell.y * cellSize)) ;
	double near_z = (step_Z >= 0) ? ((startCell.z + 1) * cellSize - startPoint.z) : (startPoint.z - (startCell.z * cellSize)) ;

	double maxX = (norm_direction.x != 0) ? near_x / norm_direction.x : DOUBLE_MAX;
	double maxY = (norm_direction.y != 0) ? near_y / norm_direction.y : DOUBLE_MAX;
	double masZ = (norm_direction.z != 0) ? near_z / norm_direction.z : DOUBLE_MAX;

	double dx = (norm_direction.x != 0) ? cellSize / norm_direction.x : DOUBLE_MAX;
	double dy = (norm_direction.y != 0) ? cellSize / norm_direction.y : DOUBLE_MAX;
	double dz = (norm_direction.z != 0) ? cellSize / norm_direction.z : DOUBLE_MAX;

	glm::vec3 pos = startPoint; 
	positions.push_back(startPoint);

	int gridBoundX = std::abs(endCell.x - startCell.x);
	int gridBoundY = std::abs(endCell.y - startCell.y);
	int gridBoundZ = std::abs(endCell.z - startCell.z);

	int counter = 0;

	while(counter != (gridBoundX + gridBoundY + gridBoundZ)) {
		if(std::abs(maxX) < std::abs(maxY)) {
			if(std::abs(maxX) < std::abs(masZ)) {
				maxX += dx;
				pos.x += step_X;
			} else {
				masZ += dz;
				pos.z += step_Z;
			}
		} else {
			if(std::abs(maxY) < std::abs(masZ)) {
				maxY += dy;
				pos.y += step_Y;
			} else {
				masZ += dz;
				pos.z += step_Z;
			}
		}

		BlockIDs block = World::GetChunkManager().GetBlock(pos);
		
		if(pos.y < 0)
			break; //return Ray::RayHit(BlockIDs::AIR, glm::vec3(0), BlockIDs::AIR, glm::vec3(0), false);

		if(block == BlockIDs::AIR || block == BlockIDs::WATER) {
			++counter;
			positions.push_back(glm::floor(pos));
			continue;
		}

		return Ray::RayHit(block, glm::floor(pos), BlockIDs::AIR, (positions.size() != 0) ? positions[positions.size() - 1] : pos, true);
	}

	return Ray::RayHit(BlockIDs::AIR, glm::vec3(0), BlockIDs::AIR, glm::vec3(0), false);
}
