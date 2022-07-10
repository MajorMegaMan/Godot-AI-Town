
# returns the target goal this behaviour should be seeking towards
# goalArray is the array of goalResources contained in this behaviour
static func FindGoal(goalArray, _agentWorldState):
	print("Searching for Goal in GD Script...")
	return goalArray[0]

# called during every Update step of the Agent
# use this to update variables that should be updated every frame.
static func Update(_delta, _agentWorldState):
	pass
