
// Copied from https://github.com/fjordimm/cellular-automata-variations/blob/main/scripts/p5sketch.js

class CellType {
	constructor(id, name, color, ruleset) {
		this.id = id;
		this.name = name;
		this.color = color;
		this.ruleset = ruleset;
	}
}

class CellTypes {
	static size = 0;
	static list = [];

	static addCellType(name, color, ruleset) {
		const cellType = new CellType(this.size, name, color, ruleset);
		this.list[this.size] = cellType;
		this.size++;

		this.addCellTypeToRadio(cellType);
	}

	static addCellTypeToRadio(cellType) {
		const form = document.getElementById("form-paint-radio");

		const input = document.createElement("input");
		input.id = `paint-radio-${cellType.name}`;
		input.type = "radio";
		input.name = "paint-radio";
		input.value = cellType.name;
		form.appendChild(document.createTextNode(" "));
		form.appendChild(input);

		const label = document.createElement("label");
		label.htmlFor = input.id;
		label.innerHTML = cellType.name;
		form.appendChild(document.createTextNode(" "));
		form.appendChild(label);

		const colorLabel = document.createElement("div");
		colorLabel.classList.add("color-label");
		colorLabel.style.backgroundColor = cellType.color;
		form.appendChild(document.createTextNode(" "));
		form.appendChild(colorLabel);

		form.appendChild(document.createTextNode(" "));
		form.appendChild(document.createElement("br"));
	}

	static getCellType(id) {
		return this.list[id];
	}

	static getCellTypeId(name) {
		let id = 0;
		while (id < this.list.length && this.list[id].name != name) {
			id++;
		}
		return id;
	}

	static {
		this.addCellType("None", "#000000", null);

		for (let i = 0; i < userCellTypesList.length; i++) {
			const cellTypeParams = userCellTypesList[i];
			this.addCellType(cellTypeParams[0], cellTypeParams[1], cellTypeParams[2]);
		}
	}
}

class Grid {
	constructor(width, height) {
		this.width = width;
		this.height = height;

		this.mat = [];
		for (let c = 0; c < this.width; c++) {
			this.mat[c] = [];
			for (let r = 0; r < this.height; r++) {
				this.mat[c][r] = 0;
			}
		}
	}

	get(c, r) {
		if (c < 0 || c >= this.width || r < 0 || r >= this.height) {
			return 0;
		} else {
			return this.mat[c][r];
		}
	}

	set(c, r, val) {
		if (c < 0 || c >= this.width || r < 0 || r >= this.height) {
			return;
		} else {
			this.mat[c][r] = val;
		}
	}

	copyFrom(other) {
		for (let c = 0; c < this.width; c++) {
			for (let r = 0; r < this.height; r++) {
				this.mat[c][r] = other.get(c, r);
			}
		}
	}

	equals(other) {
		for (let c = 0; c < this.width; c++) {
			for (let r = 0; r < this.height; r++) {
				if (this.mat[c][r] != other.get(c, r)) return false;
			}
		}
		return true;
	}
}

let _col_background;
let _col_gridLines;

let _gridHeight = 100;
let _gridWidth = _gridHeight * 2;

const _canvasWidth = window.innerWidth * 0.6;
const _canvasHeight = _canvasWidth * 0.5;

let _cellSize = Math.min(_canvasWidth / _gridWidth, _canvasHeight / _gridHeight);

let cellColors;

let mainGrid = new Grid(_gridWidth, _gridHeight);
let oldGrid = new Grid(_gridWidth, _gridHeight);

let genCount;
let gridHistory;

let gameStarted;
let startedDrawingCells;
let cellToPaint;
let clock;

function getClockInterval(x) { return Math.floor(100 * Math.exp((x - 1) / 100 * Math.log(1/100))) }
let clockInterval = getClockInterval(document.getElementById("speed-slider").value);
document.getElementById("speed-slider").oninput = function() { clockInterval = getClockInterval(this.value); }

function setup() {
	createCanvas(_canvasWidth, _canvasHeight);

	_col_background = color(0, 0, 0);
	_col_gridLines = color(30, 30, 30);

	cellColors = [];
	for (let i = 0; i < CellTypes.size; i++) {
		cellColors[i] = color(CellTypes.getCellType(i).color);
	}
	
	// mainGrid.set(2, 1, CellTypes.getCellTypeId("Conway"));
	// mainGrid.set(3, 2, CellTypes.getCellTypeId("Conway"));
	// mainGrid.set(3, 3, CellTypes.getCellTypeId("Conway"));
	// mainGrid.set(2, 3, CellTypes.getCellTypeId("Conway"));
	// mainGrid.set(1, 3, CellTypes.getCellTypeId("Conway"));
	oldGrid.copyFrom(mainGrid);

	genCount = 0;
	gridHistory = [];
	gridHistory[0] = new Grid(_gridWidth, _gridHeight);
	gridHistory[0].copyFrom(mainGrid);

	gameStarted = false;
	startedDrawingCells = false;
	cellToPaint = CellTypes.size - 1;
	document.getElementById(`paint-radio-${CellTypes.getCellType(cellToPaint).name}`).setAttribute("checked", true);
	clock = 0;

	drawGrid();
}

function draw() {
	if (mouseIsPressed && (mouseX >= 0 && mouseX <= _canvasWidth && mouseY >= 0 && mouseY <= _canvasHeight)) {
		startedDrawingCells = true;

		let c = Math.floor(mouseX / _cellSize);
		let r = Math.floor(mouseY / _cellSize);
		mainGrid.set(c, r, cellToPaint);

		drawGrid();
	} else if (gameStarted) {
		if (clock >= clockInterval) {
			nextGrid(true);
			drawGrid();
			clock = 0;
		}
		clock++;
	}
}

function mouseReleased() {
	if (!startedDrawingCells) return;
	dummyNextGrid();
	startedDrawingCells = false;
	drawGrid();
}

function keyPressed() {
	if (keyCode >= 48 && keyCode <= 57) {
		const id = keyCode - 48;
		if (id < CellTypes.size) {
			cellToPaint = id;
			document.forms['form-paint-radio']['paint-radio'].value = CellTypes.getCellType(cellToPaint).name;
		}
	} else if (keyCode == 192 /* backtick */) {
		cellToPaint = 0;
		document.forms['form-paint-radio']['paint-radio'].value = CellTypes.getCellType(cellToPaint).name;
	} else {
		if (keyCode == 32 /* space */) {
			document.getElementById("start-button").onclick();
		} else if (keyCode == 37 /* left arrow */) {
			document.getElementById("prev-button").onclick();
		} else if (keyCode == 39 /* right arrow */) {
			document.getElementById("next-button").onclick();
		} else if (keyCode == 88 /* X */) {
			document.getElementById("clear-button").onclick();
		}
	}
}

function drawGrid() {
	background(_col_background);
	
	noStroke();
	for (let c = 0; c < _gridWidth; c++) {
		for (let r = 0; r < _gridHeight; r++) {
			let cellId = mainGrid.get(c, r);
			if (cellId != 0) {
				fill(cellColors[cellId]);
				rect(c * _cellSize, r * _cellSize, _cellSize, _cellSize);
			} else if (gridHistory.length > 1) {
				let prev1CellId = gridHistory[gridHistory.length - 2].get(c, r);
				if (prev1CellId != 0) {
					fill(lerpColor(cellColors[0], cellColors[prev1CellId], 0.2));
					rect(c * _cellSize, r * _cellSize, _cellSize, _cellSize);
				} else if (gridHistory.length > 2) {
					let prev2CellId = gridHistory[gridHistory.length - 3].get(c, r);
					if (prev2CellId != 0) {
						fill(lerpColor(cellColors[0], cellColors[prev2CellId], 0.1));
						rect(c * _cellSize, r * _cellSize, _cellSize, _cellSize);
					}
				}
			}
		}
	}

	strokeWeight(1);
	stroke(_col_gridLines);
	noFill();
	for (let c = 0; c < _gridWidth + 1; c++) {
		line(c * _cellSize, 0, c * _cellSize, _canvasHeight);
	}
	for (let r = 0; r < _gridHeight + 1; r++) {
		line(0, r * _cellSize, _canvasWidth, r * _cellSize);
	}
}

function nextGrid(stopOnRedundance=false) {
	for (let c = 0; c < _gridWidth; c++) {
		for (let r = 0; r < _gridHeight; r++) {
			mainGrid.set(c, r, 0);
			for (let i = 1; i < CellTypes.size; i++) {
				const res = CellTypes.getCellType(i).ruleset(oldGrid, c, r, 0, i, CellTypes);
				if (res) {
					mainGrid.set(c, r, i);
				}
			}
		}
	}

	if (stopOnRedundance && oldGrid.equals(mainGrid)) {
		gameStarted = false;
		document.getElementById("start-checkbox").checked = false;
	} else {
		oldGrid.copyFrom(mainGrid);
	
		genCount++;
		document.getElementById("gen-count").innerHTML = genCount;
	
		gridHistory[genCount] = new Grid(_gridWidth, _gridHeight);
		gridHistory[genCount].copyFrom(mainGrid);
	}

	if (genCount > 0) {
		document.getElementById("prev-button").disabled = false;
	}
}

function dummyNextGrid() {
	genCount++;
	document.getElementById("gen-count").innerHTML = genCount;

	gridHistory[genCount] = new Grid(_gridWidth, _gridHeight);
	gridHistory[genCount].copyFrom(mainGrid);
	oldGrid.copyFrom(mainGrid);

	document.getElementById("prev-button").disabled = false;
}

function prevGrid() {
	gridHistory.pop();

	genCount--;
	document.getElementById("gen-count").innerHTML = genCount;

	if (genCount <= 0) {
		document.getElementById("prev-button").disabled = true;
	}

	mainGrid.copyFrom(gridHistory[genCount]);
	oldGrid.copyFrom(gridHistory[genCount]);
}

function revertGrid() {
	gridHistory = [gridHistory[0]];

	genCount = 0;
	document.getElementById("gen-count").innerHTML = genCount;

	document.getElementById("prev-button").disabled = true;

	mainGrid.copyFrom(gridHistory[genCount]);
	oldGrid.copyFrom(gridHistory[genCount]);
}

function saveGrid() {
	gridHistory = [gridHistory[genCount]];

	genCount = 0;
	document.getElementById("gen-count").innerHTML = genCount;

	document.getElementById("prev-button").disabled = true;

	mainGrid.copyFrom(gridHistory[genCount]);
	oldGrid.copyFrom(gridHistory[genCount]);
}

document.getElementById("prev-button").onclick = function() {
	if (genCount > 0) {
		prevGrid();
		drawGrid();
	}
}

document.getElementById("start-button").onclick = function() {
	if (document.getElementById("start-checkbox").checked == false) {
		gameStarted = true;
		document.getElementById("start-checkbox").checked = true;
	} else {
		gameStarted = false;
		document.getElementById("start-checkbox").checked = false;
	}
}

document.getElementById("next-button").onclick = function() {
	nextGrid();
	drawGrid();
}

document.getElementById("revert-button").onclick = function() {
	revertGrid();
	drawGrid();
}

document.getElementById("save-button").onclick = function() {
	saveGrid();
	drawGrid();
}

document.getElementById("clear-button").onclick = function() {
	for (let c = 0; c < _gridWidth; c++) {
		for (let r = 0; r < _gridHeight; r++) {
			mainGrid.set(c, r, 0);
		}
	}
	dummyNextGrid();
	drawGrid();
}

document.getElementById("form-paint-radio").onchange = function() {
	const name = document.forms['form-paint-radio']['paint-radio'].value;
	cellToPaint = CellTypes.getCellTypeId(name);
}

// Makes it so hitting space or enter doesn't activate the buttons
document.querySelectorAll("button").forEach( function(item) {
    item.addEventListener('focus', function() {
        this.blur();
    })
})