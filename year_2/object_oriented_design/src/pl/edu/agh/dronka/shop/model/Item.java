package pl.edu.agh.dronka.shop.model;

import java.time.LocalDate;
import java.util.Date;

public class Item {

	private String name;

	private Category category;

	private int price;

	private int quantity;

	private boolean secondhand;

	private boolean polish;
	private boolean hardCover;
	private int noPages;
	private boolean mobile;
	private boolean warranty;
	private LocalDate expiryDate;
	private boolean videoIncluded;
	private Genre musicGenre;

	public Item(String name, Category category, int price, int quantity) {
		this.name = name;
		this.category = category;
		this.price = price;
		this.quantity = quantity;
	}

	public Item() {
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public void setCategory(Category category) {
		this.category = category;
	}

	public Category getCategory() {
		return category;
	}

	public int getPrice() {
		return price;
	}

	public int getQuantity() {
		return quantity;
	}

	public void setSecondhand(boolean secondhand) {
		this.secondhand = secondhand;
	}

	public boolean isSecondhand() {
		return secondhand;
	}

	public void setPolish(boolean polish) {
		this.polish = polish;
	}

	public boolean isPolish() {
		return polish;
	}

	public boolean isHardCover() {
		return hardCover;
	}

	public void setHardCover(boolean hardCover) {
		this.hardCover = hardCover;
	}

	public int getNoPages() {
		return noPages;
	}

	public void setNoPages(int noPages) {
		this.noPages = noPages;
	}

	public boolean isMobile() {
		return mobile;
	}

	public void setMobile(boolean mobile) {
		this.mobile = mobile;
	}

	public boolean isWarranty() {
		return warranty;
	}

	public void setWarranty(boolean warranty) {
		this.warranty = warranty;
	}

	public LocalDate getExpiryDate() {
		return expiryDate;
	}

	public void setExpiryDate(LocalDate expiryDate) {
		this.expiryDate = expiryDate;
	}

	public boolean isVideoIncluded() {
		return videoIncluded;
	}

	public void setVideoIncluded(boolean videoIncluded) {
		this.videoIncluded = videoIncluded;
	}

	public Genre getMusicGenre() {
		return musicGenre;
	}

	public void setMusicGenre(Genre musicGenre) {
		this.musicGenre = musicGenre;
	}

	@Override
	public String toString() {
		return getName();
	}
}
