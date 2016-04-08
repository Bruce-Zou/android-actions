package com.test.pcbatest;

public class Test {
	private Integer id;
	private String name;
	private String usable;
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getUsable() {
		return usable;
	}
	public void setUsable(String usable) {
		this.usable = usable;
	}
	@Override
	public String toString() {
		return "Item [id=" + id + ", name=" + name + ", usable=" + usable + "]";
	}
	public Test(Integer id, String name, String usable) {		
		this.id = id;
		this.name = name;
		this.usable = usable;
	}	
	public Test() {	
		
	}	
}
