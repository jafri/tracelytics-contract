import { Contract } from 'lamington';

// Common
export interface ItemProductQuantitySingle {
  item    : string;
  product : string;
  quantity: number;
  metadata: object;
}
export interface ProductQuantitySingle {
  product : string;
  quantity: number;
  metadata: object;
}

export type ProductQuantities = ItemProductQuantitySingle[]

export interface ProductQuantity {
  // Item is the key
  [key: string]: ProductQuantitySingle
}

export interface Metadata {
  [key: string]: string
}

// Table row types
export interface Delivery {
	index      : number;
	company    : string;
	deliveryId : string;
	type       : string;
	fromSite   : string;
	toSite     : string;
	startTime  : string;
	endTime    : string;
	shipper    : string;
	driver     : string;
	status     : string;
	description: string;
	version    : string;
	createdBy  : string;
	updatedBy  : string;
	createdAt  : string;
	updatedAt  : string;
	cargo      : ProductQuantities;
	data       : Array<any>;
}

export interface Inventorylog {
	index         : number;
	user          : string;
	company       : string;
	item          : string;
	site          : string;
	product       : string;
	metadata      : Metadata;
	action        : string;
	parentAction  : string;
	parentActionId: string;
	timestamp     : string;
	oldQuantity   : number;
	newQuantity   : number;
	delta         : number;
	txid          : string;
	version       : string;
	data          : Array<any>;
}

export interface Item {
	index    : number;
	company  : string;
	site     : string;
	itemId   : string;
	product  : string;
	quantity : number;
	createdBy: string;
	updatedBy: string;
	createdAt: string;
	updatedAt: string;
	metadata : Metadata;
	version  : string;
	data     : Array<any>;
}

export interface Machine {
	index         : number;
	company       : string;
	machineId     : string;
	name          : string;
	site          : string;
	description   : string;
	createdBy     : string;
	updatedBy     : string;
	createdAt     : string;
	updatedAt     : string;
	version       : string;
	certifications: Array<string>;
	data          : Array<any>;
}

export interface Process {
	index      : number;
	company    : string;
	processId  : string;
	type       : string;
	machine    : string;
	site       : string;
	startTime  : string;
	endTime    : string;
	status     : string;
	description: string;
	createdBy  : string;
	updatedBy  : string;
	createdAt  : string;
	updatedAt  : string;
	version    : string;
	inputs     : ProductQuantities;
	outputs    : ProductQuantities;
	data       : Array<any>;
}

export interface Product {
	index      : number;
	company    : string;
	productId  : string;
	name       : string;
	uom        : string;
	image      : string;
	description: string;
	createdBy  : string;
	updatedBy  : string;
	createdAt  : string;
	updatedAt  : string;
	version    : string;
	data       : Array<any>;
}

export interface Recipe {
	index      : number;
	company    : string;
	recipeId   : string;
	name       : string;
	description: string;
	createdBy  : string;
	updatedBy  : string;
	createdAt  : string;
	updatedAt  : string;
	version    : string;
	inputs     : Array<ProductQuantitySingle>;
	outputs    : Array<ProductQuantitySingle>;
	data       : Array<any>;
}

export interface Site {
	index      : number;
	company    : string;
	siteId     : string;
	name       : string;
	description: string;
	createdBy  : string;
	updatedBy  : string;
	createdAt  : string;
	updatedAt  : string;
	version    : string;
	data       : Array<any>;
}

export interface User {
	index         : number;
	company       : string;
	userId        : string;
	key           : string;
	nonce         : number;
	firstName     : string;
	lastName      : string;
	email         : string;
	permissions   : Array<string>;
	certifications: Array<string>;
	description   : string;
	createdBy     : string;
	updatedBy     : string;
	createdAt     : string;
	updatedAt     : string;
	version       : string;
	data          : Array<any>;
}

export interface TracelyticsAbi extends Contract {
	// Actions
	deldelivery  (data: { user: string, company: string, deliveryId: string, timestamp: string })                                                                                                                                                                                                                   : Promise<any>;
	delitem      (data: { user: string, company: string, site: string, itemId: string, action: string, actionId: string, timestamp: string })                                                                                                                                                                       : Promise<any>;
	delmachine   (data: { user: string, company: string, machineId: string, timestamp: string })                                                                                                                                                                                                                    : Promise<any>;
	delprocess   (data: { user: string, company: string, processId: string, timestamp: string })                                                                                                                                                                                                                    : Promise<any>;
	delproduct   (data: { user: string, company: string, productId: string, timestamp: string })                                                                                                                                                                                                                    : Promise<any>;
	delrecipe    (data: { user: string, company: string, recipeId: string, timestamp: string })                                                                                                                                                                                                                     : Promise<any>;
	delsite      (data: { user: string, company: string, siteId: string, timestamp: string })                                                                                                                                                                                                                       : Promise<any>;
	deluser      (data: { user: string, company: string, userId: string, timestamp: string })                                                                                                                                                                                                                       : Promise<any>;
	editdelivery (data: { user: string, company: string, deliveryId: string, cargoDelta: ProductQuantities, timestamp: string, data: Array<any>, toSite: string, type: string, startTime: string, endTime: string, shipper: string, driver: string, status: string, description: string, version: string })         : Promise<any>;
	edititem     (data: { user: string, company: string, site: string, itemId: string, metadata: Metadata, action: string, actionId: string, timestamp: string, data: Array<any>, quantity: string, delta: string, product: string, version: string })                                                              : Promise<any>;
	editmachine  (data: { user: string, company: string, machineId: string, timestamp: string, data: Array<any>, site: string, name: string, description: string, version: string })                                                                                                                                : Promise<any>;
	editprocess  (data: { user: string, company: string, processId: string, inputsDelta: ProductQuantities, outputsDelta: ProductQuantities, timestamp: string, data: Array<any>, startTime: string, endTime: string, machine: string, status: string, description: string, version: string })                      : Promise<any>;
	editproduct  (data: { user: string, company: string, productId: string, timestamp: string, data: Array<any>, uom: string, name: string, image: string, description: string, version: string })                                                                                                                  : Promise<any>;
	editrecipe   (data: { user: string, company: string, recipeId: string, inputs: Array<ProductQuantitySingle>, outputs: Array<ProductQuantitySingle>, timestamp: string, data: Array<any>, name: string, description: string, version: string })                                                                  : Promise<any>;
	editsite     (data: { user: string, company: string, siteId: string, timestamp: string, data: Array<any>, name: string, description: string, version: string })                                                                                                                                                 : Promise<any>;
	edituser     (data: { user: string, company: string, userId: string, permissions: Array<string>, certifications: Array<string>, timestamp: string, data: Array<any>, key: string, firstName: string, lastName: string, email: string, description: string, version: string })                                   : Promise<any>;
	loginventory (data: { user: string, company: string, item: string, site: string, product: string, metadata:  Metadata, action: string, parentAction: string, parentActionId: string, timestamp: string, version: string, oldQuantity: number, newQuantity: number })                                            : Promise<any>;
	newdelivery  (data: { user: string, company: string, deliveryId: string, type: string, fromSite: string, toSite: string, startTime: string, cargo: ProductQuantities, timestamp: string, data: Array<any>, endTime: string, shipper: string, driver: string, status: string, description: string, version: string }): Promise<any>;
	newitem      (data: { user: string, company: string, site: string, itemId: string, product: string, quantity: number, metadata:  Metadata, action: string, actionId: string, timestamp: string, data: Array<any>, version: string })                                                                            : Promise<any>;
	newmachine   (data: { user: string, company: string, machineId: string, site: string, timestamp: string, data: Array<any>, name: string, description: string, version: string })                                                                                                                                : Promise<any>;
	newprocess   (data: { user: string, company: string, processId: string, type: string, site: string, startTime: string, inputs: ProductQuantities, outputs: ProductQuantities, timestamp: string, data: Array<any>, endTime: string, machine: string, status: string, description: string, version: string })    : Promise<any>;
	newproduct   (data: { user: string, company: string, productId: string, uom: string, timestamp: string, data: Array<any>, name: string, image: string, description: string, version: string })                                                                                                                  : Promise<any>;
	newrecipe    (data: { user: string, company: string, recipeId: string, inputs: Array<ProductQuantitySingle>, outputs: Array<ProductQuantitySingle>, timestamp: string, data: Array<any>, name: string, description: string, version: string })                                                                  : Promise<any>;
	newsite      (data: { user: string, company: string, siteId: string, timestamp: string, data: Array<any>, name: string, description: string, version: string })                                                                                                                                                 : Promise<any>;
	newuser      (data: { user: string, company: string, userId: string, permissions: Array<string>, certifications: Array<string>, timestamp: string, data: Array<any>, key: string, firstName: string, lastName: string, email: string, description: string, version: string })                                   : Promise<any>;
}

